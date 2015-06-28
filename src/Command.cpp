/**
 * @file   Command.cpp
 * @brief  �R�}���h�I�u�W�F�N�g
 * @author kyo
 * @date   2015/05/01
 */
#include "Command.h"
#include <memory>
#include <string>
#include <vector>

/** Analysis Drive */
namespace adlib
{
//! Pimpl�C�f�B�I��
class Command::Impl
{
public:
  //! �ړ�����
  const std::string m_prefix;
  //! ����
  const std::vector<std::string> m_arguments;

  /**
   * �R���X�g���N�^
   * @param[in] prefix    �ړ�����
   * @param[in] arguments ����
   */
  Impl(const std::string& prefix, const std::vector<std::string>& arguments)
    :m_prefix(prefix),
     m_arguments(arguments)
  {
  }

  /**
   * ���[�u�R���X�g���N�^
   * @param[in] obj ���[�u�I�u�W�F�N�g
   */
  Impl(Impl&& obj)
    :m_prefix(std::move(obj.m_prefix)),
     m_arguments(std::move(obj.m_arguments))
  {
  }
};

/** �R���X�g���N�^ */
Command::Command()
  :m_impl(std::make_unique<Impl>("", std::vector<std::string>()))
{
}

/**
 * �R���X�g���N�^
 * @param[in] prefix    �ړ�����
 * @param[in] arguments ����
 */
Command::Command(const std::string& prefix, const std::vector<std::string>& arguments)
  :m_impl(std::make_unique<Impl>(prefix, arguments))
{
}

/**
 * ���[�u�R���X�g���N�^
 * @param[in] obj ���[�u�I�u�W�F�N�g
 */
Command::Command(Command&& obj)
  :m_impl(std::move(m_impl))
{
}

/** �f�X�g���N�^ */
Command::~Command() {}

/**
 * �ړ�������get
 * @return �ړ�����
 */
const std::string& Command::Prefix() const
{
  return m_impl->m_prefix;
}

/**
 * �������i�[���ꂽ�R���e�i�̎Q�Ƃ�get
 * @return �����Q��
 */
const std::vector<std::string>& Command::Args() const
{
  return m_impl->m_arguments;
}
}
