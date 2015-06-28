/**
 * @file   CommandLineParser.cpp
 * @brief  �R�}���h���C���ǂݍ��݋@�\
 * @author kyo
 * @date   2015/05/01
 */
#include "CommandLineParser.h"
#include "CommandLineSetting.h"
#include "Command.h"
#include <memory>
#include <vector>
#include <string>
#include <map>

/** Analysis Drive */
namespace adlib
{
//! Pimpl�C�f�B�I��
class CommandLineParser::Impl
{
public:
  //! �Z���[���b�Z�[�W���i�[����o�b�t�@�T�C�Y
  static const size_t BUFFER_SIZE;
  //! �G���[���b�Z�[�W --- �ړ������d�����Đݒ肳��Ă���(���C�u�������p�҂̃G���[)
  static const std::string ERR_MSG_SETTING_DOUBLE_PREFIX;
  //! �G���[���b�Z�[�W --- �R�}���h���C�������̓��e���Ȃ�(���s�t�@�C�������Ȃ�. ���C�u�������p�҂̃G���[)
  static const std::string ERR_MSG_NOT_EXIST_COMMANDLINE_CONTENTS;
  //! �G���[���b�Z�[�W --- �󔒂̐ړ���������(���C�u�������p�҂̃G���[)
  static const std::string ERR_MSG_EXIST_NOTHING_PREFIX_IN_SETTING;
  //! �G���[���b�Z�[�W --- �ړ������d�����ē��͂���Ă���
  static const std::string ERR_MSG_DOUBLE_PREFIX_INPUT;
  //! �G���[���b�Z�[�W --- �p�����[�^���s�����Ă���
  static const std::string ERR_MSG_PARAMETER_INSUFFICIENT;
  //! �G���[���b�Z�[�W --- �����ȃp�����[�^�����͂���Ă���
  static const std::string ERR_MSG_PARAMETER_INVALID;

  //! �G���[���b�Z�[�W�R���e�i
  std::vector<std::string> m_errorMsg;
  //! �R�}���h���e
  std::string m_arguments;

  //! �R���X�g���N�^
  Impl()
    :m_arguments("")
  {
  }

  /**
   * ������Z�q
   * @param[in] obj ����I�u�W�F�N�g
   */
  Impl& operator=(const Impl& obj)
  {
    m_errorMsg = obj.m_errorMsg;
    m_arguments = obj.m_arguments;
    return *this;
  }

  /**
   * �G���[���b�Z�[�W���쐬����
   * @param[in] messageTemp �G���[���b�Z�[�W
   * @param[in] keyWord     ���b�Z�[�W�ɖ��ߍ��ރL�[���[�h
   * @return �G���[���b�Z�[�W
   * @remarks messageTemp�́u%s�v�̉ӏ���keyWord�𖄂ߍ���ŕԂ�
   */
  const std::string MakeErrorMsg(const std::string& messageTemp, const std::string& keyWord) const;

  /**
   * �ݒ肳�ꂽ�ړ����̃G���[�`�F�b�N������(�G���[������ꍇ�͗�O�𓊂���)
   * @param[in] settings �R�}���h���C���ǂݍ��ݐݒ�R���e�i
   * @remarks �E�󔒂̐ړ���������΃G���[ <br>
   *          �E�d��������΃G���[
   */
  void CheckSettingPrefix(const std::vector<CommandLineSetting>& settings) const;

  /**
   * �R�}���h���e���L�^���Ă���
   * @param[in] argc argv�̔z��T�C�Y
   * @param[in] argv �R�}���h���C������
   */
  void SaveArguments(int argc, const char* const argv[]);

  /**
   * ���͂��ꂽ�R�}���h�ɐړ����̏d�����Ȃ����`�F�b�N����(�d��������ꍇ�͗�O�𓊂���)
   * @param[in] settings �R�}���h���C���ǂݍ��ݐݒ�R���e�i
   * @param[in] argc argv�̔z��T�C�Y
   * @param[in] argv �R�}���h���C������
   * @return �ł����̐ړ������̉e���͈�(�ړ������̃C���f�b�N�X + ���Ɏ������̐�)
   */
  int CheckDoublePrefixInput(
    const std::vector<CommandLineSetting>& settings,
    int argc,
    const char* const argv[]) const;

  /**
   * �R�}���h�I�u�W�F�N�g�̘A�z�z����쐬����
   * @param[in] settings            �R�}���h���C���ǂݍ��ݐݒ�R���e�i
   * @param[in] argc                argv�̔z��T�C�Y
   * @param[in] argv                �R�}���h���C������
   * @param[in] indexLastPrefixArea �ł����̐ړ������̉e���͈�
   */
  const std::map<std::string, Command> MakeCommandArray(
    const std::vector<CommandLineSetting>& settings,
    int argc,
    const char* const argv[],
    int indexLastPrefixArea) const;
};

//! �G���[���b�Z�[�W���i�[����o�b�t�@�T�C�Y
const size_t CommandLineParser::Impl::BUFFER_SIZE = 256;
//! �G���[���b�Z�[�W --- �ړ������d�����Đݒ肳��Ă���(���C�u�������p�҂̃G���[)
const std::string CommandLineParser::Impl::ERR_MSG_SETTING_DOUBLE_PREFIX("(Program Error) Prefix \"%s\" is Defined Double.");
//! �G���[���b�Z�[�W --- �R�}���h���C�������̓��e���Ȃ�(���s�t�@�C�������Ȃ�. ���C�u�������p�҂̃G���[)
const std::string CommandLineParser::Impl::ERR_MSG_NOT_EXIST_COMMANDLINE_CONTENTS("(Program Error) There is no Commandline Contents.");
//! �G���[���b�Z�[�W --- �󔒂̐ړ���������(���C�u�������p�҂̃G���[)
const std::string CommandLineParser::Impl::ERR_MSG_EXIST_NOTHING_PREFIX_IN_SETTING("(Program Error) There is Prefix \"\" in Commandline Setting.");
//! �G���[���b�Z�[�W --- �ړ������d�����ē��͂���Ă���
const std::string CommandLineParser::Impl::ERR_MSG_DOUBLE_PREFIX_INPUT("Prefix \"%s\" is Input Double.");
//! �G���[���b�Z�[�W --- �p�����[�^���s�����Ă���
const std::string CommandLineParser::Impl::ERR_MSG_PARAMETER_INSUFFICIENT("Parameter of Prefix \"%s\" is Insufficient.");
//! �G���[���b�Z�[�W --- �����ȃp�����[�^�����͂���Ă���
const std::string CommandLineParser::Impl::ERR_MSG_PARAMETER_INVALID("Parameter \"%s\" is Invalid.");

/** �R���X�g���N�^ */
CommandLineParser::CommandLineParser()
  :m_impl(std::make_unique<Impl>())
{
}

/** �f�X�g���N�^ */
CommandLineParser::~CommandLineParser() {}

/**
 * �G���[���b�Z�[�W���쐬����
 * @param[in] messageTemp �G���[���b�Z�[�W
 * @param[in] keyWord     ���b�Z�[�W�ɖ��ߍ��ރL�[���[�h
 * @return �G���[���b�Z�[�W
 * @remarks messageTemp�́u%s�v�̉ӏ���keyWord�𖄂ߍ���ŕԂ�
 */
const std::string CommandLineParser::Impl::MakeErrorMsg(const std::string& messageTemp, const std::string& keyWord) const
{
  // �o�b�t�@��p��
  char buf[Impl::BUFFER_SIZE];
  // �G���[���b�Z�[�W���쐬���ĕԂ�
  sprintf_s(buf, sizeof(buf), messageTemp.c_str(), keyWord.c_str());
  return std::string(buf);
}

/**
 * �ݒ肳�ꂽ�ړ����̃G���[�`�F�b�N������(�G���[������ꍇ�͗�O�𓊂���)
 * @param[in] settings �R�}���h���C���ǂݍ��ݐݒ�R���e�i
 * @remarks �E�󔒂̐ړ���������΃G���[ <br>
 *          �E�d��������΃G���[
 */
void CommandLineParser::Impl::CheckSettingPrefix(const std::vector<CommandLineSetting>& settings) const
{
  //----- �ݒ萔���[�v -----//
  for (size_t i = 0; i < settings.size(); ++i)
  {
    // �󔒂̐ړ���������΃G���[���b�Z�[�W�𓊂���
    if (settings[i].Prefix() == "") throw ERR_MSG_EXIST_NOTHING_PREFIX_IN_SETTING;
    // �ړ������̏d�����Ȃ����m�F
    for (size_t j = i + 1; j < settings.size(); ++j)
    {
      // �d��������΃G���[���b�Z�[�W�𓊂���
      if (settings[i].Prefix() == settings[j].Prefix())
      {
        throw MakeErrorMsg(ERR_MSG_SETTING_DOUBLE_PREFIX, settings[i].Prefix().c_str());
      }
    }
  }
}

/**
 * �R�}���h���e���L�^���Ă���
 * @param[in] argc argv�̔z��T�C�Y
 * @param[in] argv �R�}���h���C������
 */
void CommandLineParser::Impl::SaveArguments(int argc, const char* const argv[])
{
  // ���s�t�@�C�������Ȃ��ꍇ�̓G���[���b�Z�[�W�𓊂���
  if (argc <= 0) throw ERR_MSG_NOT_EXIST_COMMANDLINE_CONTENTS;
  // ���s�t�@�C�������L�^
  m_arguments += argv[0];
  // ���������[�v
  for (int i = 1; i < argc; ++i)
  {
    // �R�}���h�̓��e���L�^
    m_arguments += " ";
    m_arguments += argv[i];
  }
}

/**
 * ���͂��ꂽ�R�}���h�ɐړ����̏d�����Ȃ����`�F�b�N����(�d��������ꍇ�͗�O�𓊂���)
 * @param[in] settings �R�}���h���C���ǂݍ��ݐݒ�R���e�i
 * @param[in] argc argv�̔z��T�C�Y
 * @param[in] argv �R�}���h���C������
 * @return �ł����̐ړ������̉e���͈�(�ړ������̃C���f�b�N�X + ���Ɏ������̐�)
 */
int CommandLineParser::Impl::CheckDoublePrefixInput(
  const std::vector<CommandLineSetting>& settings,
  int argc,
  const char* const argv[]) const
{
  // �ł����̐ړ������̉e���͈�(�ړ������̃C���f�b�N�X + ���Ɏ������̐�)���L�^����
  int indexLastPrefixArea = 0;
  // �ǂݍ��񂾉񐔂�ړ������ƂɋL�^����J�E���^��p��
  std::unique_ptr<int[]> inputCounts(new int[settings.size()]);
  // ������
  for (size_t j = 0; j < settings.size(); ++j) inputCounts[j] = 0;

  // ���������[�v
  for (int i = 1; i < argc; ++i)
  {
    // �ړ��������ǂ������`�F�b�N
    for (size_t j = 0; j < settings.size(); ++j)
    {
      // �ړ������̏ꍇ
      if (argv[i] == settings[j].Prefix())
      {
        // ���Ɏ󂯕t�����ړ������̏ꍇ�̓G���[
        if (inputCounts[j] > 0)
        {
          // �G���[���b�Z�[�W�𓊂���
          throw MakeErrorMsg(ERR_MSG_DOUBLE_PREFIX_INPUT, settings[j].Prefix().c_str());
        }
        // �u�ړ������̃C���f�b�N�X + ���Ɏ������̐��v���L�^
        indexLastPrefixArea = i + settings[j].NumberOfParameter();
        // �󂯕t�����񐔂��J�E���g���ă��[�v�𔲂���
        ++inputCounts[j];
        break;
      }
    }
  }
  // �ł����̐ړ������̉e���͈͂�Ԃ�
  return indexLastPrefixArea;
}

/**
 * �R�}���h�I�u�W�F�N�g�̘A�z�z����쐬����
 * @param[in] settings            �R�}���h���C���ǂݍ��ݐݒ�R���e�i
 * @param[in] argc                argv�̔z��T�C�Y
 * @param[in] argv                �R�}���h���C������
 * @param[in] indexLastPrefixArea �ł����̐ړ������̉e���͈�
 */
const std::map<std::string, Command> CommandLineParser::Impl::MakeCommandArray(
  const std::vector<CommandLineSetting>& settings,
  int argc,
  const char* const argv[],
  int indexLastPrefixArea) const
{
  // �Ԃ��A�z�z���p��
  std::map<std::string, Command> commands;

  //----- �ړ������Ȃ��̈������擾(�ł����̐ړ������̉e���͈͂̒��ォ��擾) -----//
  // �����i�[�p�R���e�i��p��
  std::vector<std::string> arguments;
  for (int i = indexLastPrefixArea + 1; i < argc; ++i)
  {
    arguments.emplace_back(argv[i]);
  }
  // �ړ������Ȃ��̈����Ƃ��ċL�^
  commands.emplace(
    std::piecewise_construct,
    std::forward_as_tuple(""),
    std::forward_as_tuple("", arguments));

  //----- ���������[�v(�ړ������Ȃ��̈����ȊO) -----//
  for (int i = 1; i <= indexLastPrefixArea; ++i)
  {
    // ���͂��ꂽ�ړ������擾
    std::string prefix = argv[i];
    // �ݒ�̒�����Y������ړ�����������
    for (auto setting = settings.begin(); setting != settings.end(); ++setting)
    {
      // �ړ��������r���Ĉ�v���Ȃ��΃X�L�b�v
      if (prefix != setting->Prefix()) continue;
      // ��v������㑱�����̐����擾
      int numArguments = setting->NumberOfParameter();
      // �㑱�����i�[�R���e�i��������
      arguments.clear();
      // �㑱���������[�v
      for (int j = 0; j < numArguments; ++j)
      {
        // �擾�ł���㑱�������Ȃ���΃G���[���b�Z�[�W�𓊂���
        if (i >= indexLastPrefixArea)
        {
          throw MakeErrorMsg(ERR_MSG_PARAMETER_INSUFFICIENT, prefix.c_str());
        }
        // �����J�E���^��i�߂�
        ++i;
        // �������i�[
        arguments.emplace_back(argv[i]);
      }
      // �����܂ŗ����烋�[�v�𔲂���
      goto SUCCESS_SEARCH;
    }
    // �������s���̓G���[���b�Z�[�W�𓊂���
    throw MakeErrorMsg(ERR_MSG_PARAMETER_INVALID, prefix.c_str());

    //----- �����������͂����ɂ��� -----//
    SUCCESS_SEARCH:
    // ���������L�^
    commands.emplace(
      std::piecewise_construct,
      std::forward_as_tuple(prefix),
      std::forward_as_tuple(prefix, arguments));
  }

  // �쐬�����A�z�z���Ԃ�
  return commands;
}

/**
 * �R�}���h���C���������p�[�X����
 * @param[in] settings �R�}���h���C���ǂݍ��ݐݒ�R���e�i
 * @param[in] argc     argv�̔z��T�C�Y
 * @param[in] argv     �R�}���h���C������
 * @return �R�}���h�I�u�W�F�N�g�̘A�z�z��
 * @attention ����܂ł̃G���[���b�Z�[�W�⃏�[�j���O���b�Z�[�W�������������
 */
const std::map<std::string, Command> CommandLineParser::Parse(
  const std::vector<CommandLineSetting>& settings,
  int argc,
  const char* const argv[])
{
  // �O��ǂݍ��񂾏�������������
  m_impl = std::make_unique<Impl>();

  try
  {
    // �R�}���h���e���L�^���Ă���
    m_impl->SaveArguments(argc, argv);
    // �ݒ肳�ꂽ�ړ����̃`�F�b�N
    m_impl->CheckSettingPrefix(settings);
    // �����̐ړ����̏d�����`�F�b�N��, �ł����̐ړ������̉e���͈͂��擾
    int indexLastPrefixArea = m_impl->CheckDoublePrefixInput(settings, argc, argv);
    // �R�}���h�I�u�W�F�N�g�̘A�z�z����쐬���ĕԂ�
    return m_impl->MakeCommandArray(settings, argc, argv, indexLastPrefixArea);
  }
  catch (const std::string& err)
  {
    // �G���[�R�[�h���i�[���ċ�̘A�z�z���Ԃ�
    m_impl->m_errorMsg.emplace_back(err);
    return std::map<std::string, Command>();
  }

  // ��̘A�z�z���Ԃ�
  return std::map<std::string, Command>();
}

/**
 * �O��Parse�œǂ񂾃R�}���h���e�𕶎����get
 * @return �R�}���h���e(exe���܂�)
 */
const std::string& CommandLineParser::GetCommandArguments() const
{
  return m_impl->m_arguments;
}

/**
 * �G���[���b�Z�[�W���擾����
 * @return �G���[���b�Z�[�W�̃R���e�i(����ł���΃T�C�Y0)
 */
const std::vector<std::string>& CommandLineParser::ErrorMessage() const
{
  return m_impl->m_errorMsg;
}
}
