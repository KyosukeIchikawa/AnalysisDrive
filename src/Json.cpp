/**
 * @file   Json.cpp
 * @brief  JSONオブジェクト
 * @author kyo
 * @date   2015/05/03
 */
#include "Json.h"
#include "JsonValue.h"
#include "JsonValueNull.h"
#include "JsonValueString.h"
#include "JsonValueInt.h"
#include "JsonValueDouble.h"
#include "JsonValueBool.h"
#include "JsonValueArray.h"
#include "JsonValueObject.h"
#include <memory>
#include <string>
#include <vector>
#include <map>

/** Analysis Drive */
namespace adlib
{
//! Pimplイディオム
class Json::Impl
{
private:
  /* デフォルトコンストラクタ禁止 */
  Impl();

public:
  //! JSONの値
  std::shared_ptr<JsonValue> m_jsonValue;

  /**
   * コンストラクタ
   * @param[in] value 値(null)
   */
  explicit Impl(std::nullptr_t value)
    :m_jsonValue(std::make_shared<JsonValueNull>(value))
  {
  }

  /**
   * コンストラクタ
   * @param[in] value 値(string型)
   */
  explicit Impl(const std::string& value)
    :m_jsonValue(std::make_shared<JsonValueString>(value))
  {
  }

  /**
   * コンストラクタ
   * @param[in] value 値(number型(int))
   */
  explicit Impl(int value)
    :m_jsonValue(std::make_shared<JsonValueInt>(value))
  {
  }

  /**
   * コンストラクタ
   * @param[in] value 値(number型(double))
   */
  explicit Impl(double value)
    :m_jsonValue(std::make_shared<JsonValueDouble>(value))
  {
  }

  /**
   * コンストラクタ
   * @param[in] value 値(bool型)
   */
  explicit Impl(bool value)
    :m_jsonValue(std::make_shared<JsonValueBool>(value))
  {
  }

  /**
   * コンストラクタ
   * @param[in] value 値(array型)
   */
  explicit Impl(const std::vector<Json>& value)
    :m_jsonValue(std::make_shared<JsonValueArray>(value))
  {
  }

  /**
   * コンストラクタ
   * @param[in] value 値(object型)
   */
  explicit Impl(const std::map<std::string, Json>& value)
    :m_jsonValue(std::make_shared<JsonValueObject>(value))
  {
  }

  /**
   * コピーコンストラクタ
   * @param[in] obj コピー元
   */
  Impl(const Impl& obj)
    :m_jsonValue(obj.m_jsonValue)
  {
  }

  /**
   * ムーブコンストラクタ
   * @param[in] obj ムーブオブジェクト
   */
  Impl(Impl&& obj)
    :m_jsonValue(std::move(obj.m_jsonValue))
  {
  }

  /**
   * 代入
   * @param[in] obj 代入するオブジェクト
   */
  Impl& Impl::operator=(const Impl& obj)
  {
    m_jsonValue = obj.m_jsonValue;
    return *this;
  }
};

/**
 * コンストラクタ
 * @remarks 引数なしのコンストラクタではnullのJSONオブジェクトを作成する
 * @note std::map<std::string, Json>を存在しないキーでアクセスした場合, <br>
 *       本コンストラクタによってJsonが新規登録される. <br>
 *       これにより, 存在しないキーでアクセスするとnullと同様の値になる.
 */
Json::Json() :m_impl(std::make_unique<Impl>(nullptr)) {}

/**
 * コンストラクタ
 * @param[in] value 値(null)
 */
Json::Json(std::nullptr_t value) :m_impl(std::make_unique<Impl>(value)) {}

/**
 * コンストラクタ
 * @param[in] value 値(string型)
 */
Json::Json(const std::string& value) :m_impl(std::make_unique<Impl>(value)) {}

/**
 * コンストラクタ
 * @param[in] value 値(number型(int))
 */
Json::Json(int value) :m_impl(std::make_unique<Impl>(value)) {}

/**
 * コンストラクタ
 * @param[in] value 値(number型(double))
 */
Json::Json(double value) :m_impl(std::make_unique<Impl>(value)) {}

/**
 * コンストラクタ
 * @param[in] value 値(bool型)
 */
Json::Json(bool value) :m_impl(std::make_unique<Impl>(value)) {}

/**
 * コンストラクタ
 * @param[in] value 値(array型)
 */
Json::Json(const std::vector<Json>& value) :m_impl(std::make_unique<Impl>(value)) {}

/**
 * コンストラクタ
 * @param[in] value 値(object型)
 */
Json::Json(const std::map<std::string, Json>& value) :m_impl(std::make_unique<Impl>(value)) {}

/**
 * コピーコンストラクタ
 * @param[in] obj コピー元
 */
Json::Json(const Json& obj) :m_impl(std::make_unique<Impl>(*(obj.m_impl)))
{
}

/**
 * ムーブコンストラクタ
 * @param[in] obj ムーブオブジェクト
 */
Json::Json(Json&& obj) :m_impl(std::move(obj.m_impl)) {}

/**
 * 代入
 * @param[in] obj 代入するオブジェクト
 */
Json& Json::operator=(const Json& obj)
{
  *(this->m_impl) = *(obj.m_impl);
  return *this;
}

/** 仮想デストラクタ */
Json::~Json() {}

/**
 * 型を取得
 * @return 型
 */
Json::JSON_VALUE_TYPE Json::GetValueType() const
{
  return m_impl->m_jsonValue->GetValueType();
}

/**
 * nullかどうか
 * @return nullならtrue
 */
bool Json::IsNull() const
{
  return m_impl->m_jsonValue->GetValueType() == Json::JSON_VALUE_TYPE::NUL;
}

/**
 * stringかどうか
 * @return stringならtrue
 */
bool Json::IsString() const
{
  return m_impl->m_jsonValue->GetValueType() == Json::JSON_VALUE_TYPE::STRING;
}

/**
 * number(int or double)かどうか
 * @return numberならtrue
 */
bool Json::IsNumber() const
{
  Json::JSON_VALUE_TYPE type = m_impl->m_jsonValue->GetValueType();
  return (type == Json::JSON_VALUE_TYPE::NUMBER_INT) || (type == Json::JSON_VALUE_TYPE::NUMBER_DOUBLE);
}

/**
 * number(int)かどうか
 * @return number(int)ならtrue
 */
bool Json::IsNumberInt() const
{
  return m_impl->m_jsonValue->GetValueType() == Json::JSON_VALUE_TYPE::NUMBER_INT;
}

/**
 * number(double)かどうか
 * @return number(double)ならtrue
 */
bool Json::IsNumberDouble() const
{
  return m_impl->m_jsonValue->GetValueType() == Json::JSON_VALUE_TYPE::NUMBER_DOUBLE;
}

/**
 * boolかどうか
 * @return boolならtrue
 */
bool Json::IsBool() const
{
  return m_impl->m_jsonValue->GetValueType() == Json::JSON_VALUE_TYPE::BOOL;
}

/**
 * arrayかどうか
 * @return arrayならtrue
 */
bool Json::IsArray() const
{
  return m_impl->m_jsonValue->GetValueType() == Json::JSON_VALUE_TYPE::ARRAY;
}

/**
 * objectかどうか
 * @return objectならtrue
 */
bool Json::IsObject() const
{
  return m_impl->m_jsonValue->GetValueType() == Json::JSON_VALUE_TYPE::OBJECT;
}

/**
 * string値のget
 * @return string値
 */
const std::string& Json::GetString() const
{
  return m_impl->m_jsonValue->GetString();
}

/**
 * number値(int)のget
 * @return number値(int)
 */
int Json::GetNumberInt() const
{
  return m_impl->m_jsonValue->GetNumberInt();
}

/**
 * number(double)のget
 * @return number値(double)
 */
double Json::GetNumberDouble() const
{
  return m_impl->m_jsonValue->GetNumberDouble();
}

/**
 * bool値のget
 * @return bool値
 */
bool Json::GetBool() const
{
  return m_impl->m_jsonValue->GetBool();
}

/**
 * array値のget
 * @return array値
 */
std::vector<Json>& Json::GetArray()
{
  return m_impl->m_jsonValue->GetArray();
}

/**
 * array値のget(const版)
 * @return array値
 */
const std::vector<Json>& Json::GetArray() const
{
  return m_impl->m_jsonValue->GetArray();
}

/**
 * object値のget
 * @return object値
 */
std::map<std::string, Json>& Json::GetObject()
{
  return m_impl->m_jsonValue->GetObject();
}

/**
 * object値のget(const版)
 * @return object値
 */
const std::map<std::string, Json>& Json::GetObject() const
{
  return m_impl->m_jsonValue->GetObject();
}

/**
 * JSON形式の文字列を出力
 * @param[in,out] outStream 出力先のストリーム
 */
void Json::Dump(std::ostream* outStream) const
{
  m_impl->m_jsonValue->Dump(outStream);
}

/**
 * MessagePack形式のバイナリを出力
 * @param[in,out] outStream 出力先のストリーム
 */
void Json::DumpMsgpack(std::ostream* outStream) const
{
  m_impl->m_jsonValue->DumpMsgpack(outStream);
}
}
