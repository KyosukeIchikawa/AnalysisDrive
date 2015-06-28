/**
 * @file   Command.h
 * @brief  �R�}���h�I�u�W�F�N�g
 * @author kyo
 * @date   2015/05/01
 */
#ifndef ANALYSISDRIVE_COMMAND_H_
#define ANALYSISDRIVE_COMMAND_H_

#include <memory>
#include <vector>
#include <string>

/** Analysis Drive */
namespace  adlib
{
/** �R�}���h�I�u�W�F�N�g */
class Command
{
private:
  //! Pimpl�C�f�B�I��
  class Impl;
  std::unique_ptr<Impl> m_impl;

private:
  /** �R�s�[�֎~ */
  Command(const Command&);
  /** ����֎~ */
  void operator=(const Command&);

public:
  /** �R���X�g���N�^ */
  Command();

  /**
   * �R���X�g���N�^
   * @param[in] prefix    �ړ�����
   * @param[in] arguments ����
   */
  Command(const std::string& prefix, const std::vector<std::string>& arguments);

  /**
   * ���[�u�R���X�g���N�^
   * @param[in] obj ���[�u�I�u�W�F�N�g
   */
  Command(Command&& obj);

  /** ���z�f�X�g���N�^ */
  virtual ~Command();

  /**
   * �ړ�������get
   * @return �ړ�����
   * @remarks �󕶎��̏ꍇ�͐ړ������Ȃ�
   */
  const std::string& Prefix() const;

  /**
   * �������i�[���ꂽ�R���e�i�̎Q�Ƃ�get
   * @return �����Q��
   */
  const std::vector<std::string>& Args() const;
};
}
#endif // ANALYSISDRIVE_COMMAND_H_
