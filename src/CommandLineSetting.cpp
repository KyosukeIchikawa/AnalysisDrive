/**
 * @file   CommandLineSetting.cpp
 * @brief  �R�}���h���C���ǂݍ��ݐݒ�
 * @author kyo
 * @date   2015/05/01
 */
#include "CommandLineSetting.h"
#include <memory>
#include <string>

/** Analysis Drive */
namespace adlib
{
//! Pimpl�C�f�B�I��
class CommandLineSetting::Impl
{
public:
  //! �ړ�����
  std::string m_prefix;
  //! �ړ������㑱�̈����̐�
  int m_numParameter;
  //! �w���v�\�L
  std::string m_help;

  /**
   * �R���X�g���N�^
   * @param[in] prefix       �ړ�����
   * @param[in] numParameter �ړ������㑱�̈����̐�
   * @param[in] help         �w���v�\�L
   */
  Impl(const std::string& prefix, int numParameter, const std::string& help)
    :m_prefix(prefix),
     m_numParameter(numParameter),
     m_help(help)
  {
  }

  /**
   * �R�s�[�R���X�g���N�^
   * @param[in] obj �R�s�[��
   */
  Impl(const Impl& obj)
    :m_prefix(obj.m_prefix),
     m_numParameter(obj.m_numParameter),
     m_help(obj.m_help)
  {
  }

  /**
   * ���
   * @param[in] obj ����I�u�W�F�N�g
   * @return �����̎��g
   */
  Impl& operator=(const Impl& obj)
  {
    m_prefix = obj.m_prefix;
    m_numParameter = obj.m_numParameter;
    m_help = obj.m_help;
    return *this;
  }
};

/**
 * �R���X�g���N�^
 * @param[in] prefix       �ړ�����
 * @param[in] numParameter �ړ������㑱�̈����̐�
 * @param[in] help         �w���v�\�L
 */
CommandLineSetting::CommandLineSetting(const std::string& prefix, int numParameter, const std::string& help)
  :m_impl(std::make_unique<Impl>(prefix, numParameter, help))
{
}

/*
 * �R�s�[�R���X�g���N�^
 * @param[in] obj �R�s�[��
 */
CommandLineSetting::CommandLineSetting(const CommandLineSetting& obj)
  :m_impl(std::make_unique<Impl>(obj.m_impl->m_prefix, obj.m_impl->m_numParameter, obj.m_impl->m_help))
{
}

/** �f�X�g���N�^ */
CommandLineSetting::~CommandLineSetting() {}

/**
 * ���
 * @param[in] obj ����I�u�W�F�N�g
 * @return �����̎��g
 */
CommandLineSetting& CommandLineSetting::operator=(const CommandLineSetting& obj)
{
  *(this->m_impl) = *(obj.m_impl);
  return *this;
}

/**
 * �ړ�������get
 * @return �ړ�����
 */
const std::string& CommandLineSetting::Prefix() const
{
  return m_impl->m_prefix;
}

/**
 * �ړ������㑱�̈����̐���get
 * @return �����̐�
 */
int CommandLineSetting::NumberOfParameter() const
{
  return m_impl->m_numParameter;
}

/**
 * �w���v�\�L��get
 * @return �w���v�\�L
 */
const std::string& CommandLineSetting::Help() const
{
  return m_impl->m_help;
}

/**
 * �ړ��������s��v���ǂ���
 * @param[in] obj ��r�Ώ�
 * @return �s��v�ł����true
 */
bool CommandLineSetting::operator!=(const CommandLineSetting& obj) const
{
  return m_impl->m_prefix != obj.m_impl->m_prefix;
}

/**
 * �ړ���������v���ǂ���
 * @param[in] obj ��r�Ώ�
 * @return ��v�ł����true
 */
bool CommandLineSetting::operator==(const CommandLineSetting& obj) const
{
  return !(this->operator!=(obj));
}

/**
 * �w���v��\��
 * @param[in]     settings     �R�}���h���C���ǂݍ��ݐݒ�R���e�i
 * @param[in]     noPrefixHelp �ړ������Ȃ��̈����Ɋւ���w���v�\��(�ړ������Ȃ��̈������s�v�ł���΋󕶎�"")
 * @param[in,out] outStream    �o�͐�̃X�g���[��
 */
void CommandLineSetting::ShowHelp(
  const std::vector<CommandLineSetting>& settings,
  const std::string& noPrefixHelp,
  std::ostream* outStream)
{
  //----- �p�����[�^�̐��� -----//
  // �ݒ萔���[�v
  for (auto setting = settings.begin(); setting != settings.end(); ++setting)
  {
    // �ړ������\��
    *outStream << setting->Prefix() << std::flush;
    // �㑱�̃p�����[�^���K�v�ȏꍇ�͐���\��
    if (setting->NumberOfParameter() > 0)
    {
      *outStream << " (+ " << setting->NumberOfParameter() << " parameters)" << std::flush;
    }
    *outStream << std::endl;
    // �w���v�\��
    *outStream << setting->Help() << std::endl;
    // ���s
    *outStream << std::endl;
  }
  // �ړ������Ȃ��̈����Ɋւ���w���v�\��
  if (noPrefixHelp != "")
  {
    *outStream << "NO PREFIX" << std::endl;
    // �w���v�\��
    *outStream << noPrefixHelp << std::endl;
    // ���s
    *outStream << std::endl;
  }
}
}
