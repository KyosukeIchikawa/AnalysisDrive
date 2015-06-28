/**
 * @file   EasyTimer.cpp
 * @brief  時間計測
 * @author kyo
 * @date   2015/03/15
 */
#include "EasyTimer.h"
#include <memory>
#include <chrono>

/** Analysis Drive */
namespace  adlib
{
//! Pimplイディオム
class EasyTimer::Impl
{
public:
  //! 計測開始時間
  std::chrono::system_clock::time_point m_startTime;

  //! コンストラクタ
  Impl()
    :m_startTime(std::chrono::system_clock::now())
  {
  }
};

/** コンストラクタ */
EasyTimer::EasyTimer()
  :m_impl(std::make_unique<Impl>())
{
}

/** 仮想デストラクタ */
EasyTimer::~EasyTimer() {}

/**
 * 時間計測開始
 */
void EasyTimer::StartTimer()
{
  m_impl->m_startTime = std::chrono::system_clock::now();
}

/**
 * 経過時間の取得(前回StartTimerを使用してからの経過時間)
 * @param[in] timeUnit 取得する時間の単位(デフォルトは秒)
 * @return 経過時間
 * @remarks StartTimerをまだ使用していない場合は, <br>
 *          コンストラクタが起動してからの経過時間を返す
 */
EasyTimer::TIME EasyTimer::GetDurationTime(EasyTimer::TIME_UNIT timeUnit) const
{
  // 経過時間取得
  auto durationTime = std::chrono::system_clock::now() - m_impl->m_startTime;
  // 単位分岐
  switch (timeUnit)
  {
    // 秒で返す
    case SECONDS:
      return static_cast<TIME>(std::chrono::duration_cast<std::chrono::seconds>(durationTime).count());
      // ミリ秒で返す
    case MILLI_SECONDS:
      return static_cast<TIME>(std::chrono::duration_cast<std::chrono::milliseconds>(durationTime).count());
    default:
      // 秒で返す
      return static_cast<TIME>(std::chrono::duration_cast<std::chrono::seconds>(durationTime).count());
  }
}
}
