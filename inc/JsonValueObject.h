/**
 * @file   JsonValueObject.h
 * @brief  JSONの値(object)
 * @author kyo
 * @date   2015/05/05
 */
#ifndef ANALYSISDRIVE_JSONVALUEOBJECT_H_
#define ANALYSISDRIVE_JSONVALUEOBJECT_H_

#include "JsonValue.h"
#include "Json.h"
#include <string>
#include <map>

/** Analysis Drive */
namespace adlib
{
/** JSONの値(object) */
class JsonValueObject final : public JsonValue
{
private:
  //! 値
  std::map<std::string, Json> m_value;

private:
  /** コピー禁止 */
  JsonValueObject(const JsonValueObject&);

public:
  /** コンストラクタ */
  JsonValueObject() {}

  /**
   * コンストラクタ
   * @tparam value 値
   */
  explicit JsonValueObject(const std::map<std::string, Json>& value) : m_value(value) {}

  /** 仮想デストラクタ */
  virtual ~JsonValueObject() {}

  /**
   * 代入
   * @param[in] obj 代入オブジェクト
   */
  JsonValueObject& operator=(const JsonValueObject& obj)
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
    return Json::JSON_VALUE_TYPE::OBJECT;
  }

  /**
   * object値のget
   * @return object値
   */
  std::map<std::string, Json>& GetObject() override
  {
    return m_value;
  }

  /**
   * object値のget(const版)
   * @return object値
   */
  const std::map<std::string, Json>& GetObject() const override
  {
    return m_value;
  }
};
}
#endif // ANALYSISDRIVE_JSONVALUEOBJECT_H_
