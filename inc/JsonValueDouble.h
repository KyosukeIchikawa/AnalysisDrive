/**
 * @file   JsonValueDouble.h
 * @brief  JSONの値(number(double))
 * @author kyo
 * @date   2015/05/05
 */
#ifndef ANALYSISDRIVE_JSONVALUEDOUBLE_H_
#define ANALYSISDRIVE_JSONVALUEDOUBLE_H_

#include "JsonValue.h"
#include "Json.h"

/** Analysis Drive */
namespace adlib
{
/** JSONの値(number(double)) */
class JsonValueDouble final : public JsonValue
{
private:
  //! 値
  double m_value;

private:
  /** デフォルトコンストラクタ禁止 */
  JsonValueDouble() {}
  /** コピー禁止 */
  JsonValueDouble(const JsonValueDouble&);
  /** 代入禁止 */
  void operator=(const JsonValueDouble&);

public:
  /**
   * コンストラクタ
   * @tparam value 値
   */
  explicit JsonValueDouble(double value) : m_value(value) {}

  /** 仮想デストラクタ */
  virtual ~JsonValueDouble() {}

  /**
   * 型を取得
   * @return 型
   */
  Json::JSON_VALUE_TYPE GetValueType() const override
  {
    return Json::JSON_VALUE_TYPE::NUMBER_DOUBLE;
  }

  /**
   * number値(int)のget
   * @return number値(int)
   */
  int GetNumberInt() const override
  {
    return static_cast<int>(m_value);
  }

  /**
   * number値(double)のget
   * @return number値(double)
   */
  double GetNumberDouble() const override
  {
    return m_value;
  }
};
}
#endif // ANALYSISDRIVE_JSONVALUEDOUBLE_H_
