/**
 * @file   CommandLineSetting.h
 * @brief  �R�}���h���C���ǂݍ��ݐݒ�
 * @author kyo
 * @date   2015/05/01
 */
#ifndef ANALYSISDRIVE_COMMANDLINESETTING_H_
#define ANALYSISDRIVE_COMMANDLINESETTING_H_

#include <memory>
#include <string>
#include <vector>
#include <iostream>

/** Analysis Drive */
namespace  adlib
{
/** �R�}���h���C���ǂݍ��ݐݒ� */
class CommandLineSetting final
{
private:
  //! Pimpl�C�f�B�I��
  class Impl;
  std::unique_ptr<Impl> m_impl;

private:
  /** �f�t�H���g�R���X�g���N�^�֎~ */
  CommandLineSetting();

public:
  /**
   * �R���X�g���N�^
   * @param[in] prefix       �ړ�����(�󕶎��͋֎~)
   * @param[in] numParameter �ړ������㑱�̈����̐�
   * @param[in] help         �w���v�\�L
   */
  CommandLineSetting(const std::string& prefix, int numParameter, const std::string& help);

  /*
   * �R�s�[�R���X�g���N�^
   * @param[in] obj �R�s�[��
   */
  CommandLineSetting(const CommandLineSetting& obj);

  /** ���z�f�X�g���N�^ */
  virtual ~CommandLineSetting();

  /**
   * ���
   * @param[in] obj ����I�u�W�F�N�g
   * @return �����̎��g
   */
  CommandLineSetting& operator=(const CommandLineSetting& obj);

  /**
   * �ړ�������get
   * @return �ړ�����
   */
  const std::string& Prefix() const;

  /**
   * �ړ������㑱�̈����̐���get
   * @return �����̐�
   */
  int NumberOfParameter() const;

  /**
   * �w���v�\�L��get
   * @return �w���v�\�L
   */
  const std::string& Help() const;

  /**
   * �ړ��������s��v���ǂ���
   * @param[in] obj ��r�Ώ�
   * @return �s��v�ł����true
   */
  bool operator!=(const CommandLineSetting& obj) const;

  /**
   * �ړ���������v���ǂ���
   * @param[in] obj ��r�Ώ�
   * @return ��v�ł����true
   */
  bool operator==(const CommandLineSetting& obj) const;

  /**
   * �w���v��\��
   * @param[in]     settings     �R�}���h���C���ǂݍ��ݐݒ�R���e�i
   * @param[in]     noPrefixHelp �ړ������Ȃ��̈����Ɋւ���w���v�\��(�ړ������Ȃ��̈������s�v�ł���΋󕶎�"")
   * @param[in,out] outStream    �o�͐�̃X�g���[��
   */
  static void ShowHelp(
    const std::vector<CommandLineSetting>& settings,
    const std::string& noPrefixHelp,
    std::ostream* outStream);
};
}
#endif // ANALYSISDRIVE_COMMANDLINESETTING_H_
