/**
 * @file   EasyTimer.h
 * @brief  ���Ԍv��
 * @author kyo
 * @date   2015/03/15
 */
#ifndef ANALYSISDRIVE_EASYTIMER_H_
#define ANALYSISDRIVE_EASYTIMER_H_

#include <memory>

/** Analysis Drive */
namespace adlib
{
/**
 * ���Ԍv��
 */
class EasyTimer
{
public:
  //! ����
  typedef long long TIME;
  //! ���Ԃ̒P��
  enum TIME_UNIT
  {
    SECONDS,      /*!< �b    */
    MILLI_SECONDS /*!< �~���b */
  };

private:
  class Impl;
  //! Pimpl�C�f�B�I��
  std::unique_ptr<Impl> m_impl;

public:
  /** �R���X�g���N�^ */
  EasyTimer();

  /** ���z�f�X�g���N�^ */
  virtual ~EasyTimer();

  /**
   * ���Ԍv���J�n
   */
  void StartTimer();

  /**
   * �o�ߎ��Ԃ̎擾(�O��StartTimer���g�p���Ă���̌o�ߎ���)
   * @param[in] timeUnit �擾���鎞�Ԃ̒P��(�f�t�H���g�͕b)
   * @return �o�ߎ���
   * @remarks StartTimer���܂��g�p���Ă��Ȃ��ꍇ��, <br>
   *          �R���X�g���N�^���N�����Ă���̌o�ߎ��Ԃ�Ԃ�
   */
  EasyTimer::TIME GetDurationTime(EasyTimer::TIME_UNIT timeUnit = SECONDS) const;
};
}

#endif // ANALYSISDRIVE_EASYTIMER_H_
