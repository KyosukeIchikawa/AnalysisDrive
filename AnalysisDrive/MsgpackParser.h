/**
 * @file   MsgpackParser.h
 * @brief  MessagePack�p�[�T�[
 * @author kyo
 * @date   2015/06/29
 */
#ifndef ANALYSISDRIVE_MSGPACKPARSER_H_
#define ANALYSISDRIVE_MSGPACKPARSER_H_

#include <memory>
#include <string>
#include <vector>

/** Analysis Drive */
namespace adlib
{
// �O���錾
class Json;

/**
 * MessagePack�p�[�T�[
 */
class MsgpackParser
{
private:
  //> Pimpl�C�f�B�I��
  class Impl;
  std::unique_ptr<Impl> m_impl;

private:
  /* �R�s�[�֎~ */
  MsgpackParser(const MsgpackParser&);
  /* ����֎~ */
  void operator=(const MsgpackParser&);

public:
  /** �R���X�g���N�^ */
  MsgpackParser();
  /** ���z�f�X�g���N�^ */
  virtual ~MsgpackParser();

  /**
   * MessgaePack�`���̕�������p�[�X����
   * @param[in] target �p�[�X�Ώۂ̕�����
   * @return Json�I�u�W�F�N�g
   * @attention ����܂ł̃G���[���b�Z�[�W�⃏�[�j���O���b�Z�[�W�������������
   */
  Json Parse(const std::string& target);

  /**
   * �G���[���b�Z�[�W���擾����
   * @return �G���[���b�Z�[�W�̃R���e�i(����ł���΃T�C�Y0)
   */
  const std::vector<std::string>& ErrorMessage() const;

  /**
   * ���[�j���O���b�Z�[�W���擾����
   * @return ���[�j���O���b�Z�[�W�̃R���e�i(����ł���΃T�C�Y0)
   */
  const std::vector<std::string>& WarningMessage() const;

  /**
   * Map��name���d�����Ă����true��Ԃ�
   * @return �O���Parse��Map��name���d�����Ă����true��Ԃ�
   */
  bool IsNameRepeated() const;
};
}
#endif // ANALYSISDRIVE_MSGPACKPARSER_H_
