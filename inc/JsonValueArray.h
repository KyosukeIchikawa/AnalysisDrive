/**
 * @file   JsonValueArray.h
 * @brief  JSONの値(array)
 * @author kyo
 * @date   2015/05/05
 */
#ifndef ANALYSISDRIVE_JSONVALUEARRAY_H_
#define ANALYSISDRIVE_JSONVALUEARRAY_H_

#include "JsonValue.h"
#include "Json.h"
#include <vector>

/** Analysis Drive */
namespace adlib
{
/** JSONの値(array) */
class JsonValueArray final : public JsonValue
{
private:
  //! 値
  std::vector<Json> m_value;

private:
  /** コピー禁止 */
  JsonValueArray(const JsonValueArray&);

public:
  /** コンストラクタ */
  JsonValueArray() {}

  /**
   * コンストラクタ
   * @tparam value 値
   */
  explicit JsonValueArray(const std::vector<Json>& value) : m_value(value) {}

  /** 仮想デストラクタ */
  virtual ~JsonValueArray() {}

  /**
   * 代入
   * @param[in] obj 代入オブジェクト
   */
  JsonValueArray& operator=(const JsonValueArray& obj)
  {
    m_value = obj.m_value;
    return *this;
  }

  /**
   * 型を取得
   * @return 型
   */
  Json::JSON_VALUE_TYPE GetValueType() const override
  {
    return Json::JSON_VALUE_TYPE::ARRAY;
  }

  /**
   * array値のget
   * @return array値
   */
  std::vector<Json>& GetArray() override
  {
    return m_value;
  }

  /**
   * array値のget(const版)
   * @return array値
   */
  const std::vector<Json>& GetArray() const override
  {
    return m_value;
  }
};
}
#endif // ANALYSISDRIVE_JSONVALUEARRAY_H_
