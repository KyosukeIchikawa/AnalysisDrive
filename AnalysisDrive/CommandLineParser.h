/**
 * @file   CommandLineParser.h
 * @brief  �R�}���h���C���ǂݍ��݋@�\
 * @author kyo
 * @date   2015/05/01
 */
#ifndef ANALYSISDRIVE_COMMANDLINEPARSER_H_
#define ANALYSISDRIVE_COMMANDLINEPARSER_H_

#include "CommandLineSetting.h"
#include "Command.h"
#include <memory>
#include <string>
#include <map>

/** Analysis Drive */
namespace  adlib
{
/** �R�}���h���C���ǂݍ��݋@�\ */
class CommandLineParser final
{
private:
  //! Pimpl�C�f�B�I��
  class Impl;
  std::unique_ptr<Impl> m_impl;

private:
  /** �R�s�[�֎~ */
  CommandLineParser(const CommandLineParser&);
  /** ����֎~ */
  void operator=(const CommandLineParser&);

public:
  /** �R���X�g���N�^ */
  CommandLineParser();

  /** ���z�f�X�g���N�^ */
  virtual ~CommandLineParser();

  /**
   * �R�}���h���C���������p�[�X����
   * @param[in] settings �R�}���h���C���ǂݍ��ݐݒ�R���e�i
   * @param[in] argc     argv�̔z��T�C�Y
   * @param[in] argv     �R�}���h���C������
   * @return �R�}���h�I�u�W�F�N�g�̘A�z�z��
   * @attention ����܂ł̃G���[���b�Z�[�W�⃏�[�j���O���b�Z�[�W�������������
   */
  const std::map<std::string, Command> Parse(
    const std::vector<CommandLineSetting>& settings,
    int argc,
    const char* const argv[]);

  /**
   * �O��Parse�œǂ񂾃R�}���h���e�𕶎����get
   * @return �R�}���h���e(exe���܂�)
   */
  const std::string& GetCommandArguments() const;

  /**
   * �G���[���b�Z�[�W���擾����
   * @return �G���[���b�Z�[�W�̃R���e�i(����ł���΃T�C�Y0)
   */
  const std::vector<std::string>& ErrorMessage() const;
};
}
#endif // ANALYSISDRIVE_COMMANDLINEPARSER_H_
