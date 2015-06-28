/**
 * @file   EasyTimer.cpp
 * @brief  ���Ԍv��
 * @author kyo
 * @date   2015/03/15
 */
#include "EasyTimer.h"
#include <memory>
#include <chrono>

/** Analysis Drive */
namespace  adlib
{
//! Pimpl�C�f�B�I��
class EasyTimer::Impl
{
public:
  //! �v���J�n����
  std::chrono::system_clock::time_point m_startTime;

  //! �R���X�g���N�^
  Impl()
    :m_startTime(std::chrono::system_clock::now())
  {
  }
};

/** �R���X�g���N�^ */
EasyTimer::EasyTimer()
  :m_impl(std::make_unique<Impl>())
{
}

/** ���z�f�X�g���N�^ */
EasyTimer::~EasyTimer() {}

/**
 * ���Ԍv���J�n
 */
void EasyTimer::StartTimer()
{
  m_impl->m_startTime = std::chrono::system_clock::now();
}

/**
 * �o�ߎ��Ԃ̎擾(�O��StartTimer���g�p���Ă���̌o�ߎ���)
 * @param[in] timeUnit �擾���鎞�Ԃ̒P��(�f�t�H���g�͕b)
 * @return �o�ߎ���
 * @remarks StartTimer���܂��g�p���Ă��Ȃ��ꍇ��, <br>
 *          �R���X�g���N�^���N�����Ă���̌o�ߎ��Ԃ�Ԃ�
 */
EasyTimer::TIME EasyTimer::GetDurationTime(EasyTimer::TIME_UNIT timeUnit) const
{
  // �o�ߎ��Ԏ擾
  auto durationTime = std::chrono::system_clock::now() - m_impl->m_startTime;
  // �P�ʕ���
  switch (timeUnit)
  {
    // �b�ŕԂ�
    case SECONDS:
      return static_cast<TIME>(std::chrono::duration_cast<std::chrono::seconds>(durationTime).count());
      // �~���b�ŕԂ�
    case MILLI_SECONDS:
      return static_cast<TIME>(std::chrono::duration_cast<std::chrono::milliseconds>(durationTime).count());
    default:
      // �b�ŕԂ�
      return static_cast<TIME>(std::chrono::duration_cast<std::chrono::seconds>(durationTime).count());
  }
}
}
