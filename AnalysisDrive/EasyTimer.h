/**
 * @file   EasyTimer.h
 * @brief  時間計測
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
 * 時間計測
 */
class EasyTimer
{
public:
  //! 時間
  typedef long long TIME;
  //! 時間の単位
  enum TIME_UNIT
  {
    SECONDS,      /*!< 秒    */
    MILLI_SECONDS /*!< ミリ秒 */
  };

private:
  class Impl;
  //! Pimplイディオム
  std::unique_ptr<Impl> m_impl;

public:
  /** コンストラクタ */
  EasyTimer();

  /** 仮想デストラクタ */
  virtual ~EasyTimer();

  /**
   * 時間計測開始
   */
  void StartTimer();

  /**
   * 経過時間の取得(前回StartTimerを使用してからの経過時間)
   * @param[in] timeUnit 取得する時間の単位(デフォルトは秒)
   * @return 経過時間
   * @remarks StartTimerをまだ使用していない場合は, <br>
   *          コンストラクタが起動してからの経過時間を返す
   */
  EasyTimer::TIME GetDurationTime(EasyTimer::TIME_UNIT timeUnit = SECONDS) const;
};
}

#endif // ANALYSISDRIVE_EASYTIMER_H_
