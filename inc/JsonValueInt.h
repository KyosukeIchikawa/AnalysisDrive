/**
 * @file   JsonValueInt.h
 * @brief  JSONの値(number(int))
 * @author kyo
 * @date   2015/05/05
 */
#ifndef ANALYSISDRIVE_JSONVALUEINT_H_
#define ANALYSISDRIVE_JSONVALUEINT_H_

#include "JsonValue.h"
#include "Json.h"
#include "IncludeMsgpack.h"

/** Analysis Drive */
namespace adlib
{
/** JSONの値(number(int)) */
class JsonValueInt final : public JsonValue
{
private:
  //! 値
  int m_value;

private:
  /** デフォルトコンストラクタ禁止 */
  JsonValueInt() {}
  /** コピー禁止 */
  JsonValueInt(const JsonValueInt&);
  /** 代入禁止 */
  void operator=(const JsonValueInt&);

public:
  /**
   * コンストラクタ
   * @tparam value 値
   */
  explicit JsonValueInt(int value) : m_value(value) {}

  /** 仮想デストラクタ */
  virtual ~JsonValueInt() {}

  /**
   * 型を取得
   * @return 型
   */
  Json::JSON_VALUE_TYPE GetValueType() const override
  {
    return Json::JSON_VALUE_TYPE::NUMBER_INT;
  }

  /**
   * number値(int)のget
   * @return number値(int)
   */
  int GetNumberInt() const override
  {
    return m_value;
  }

  /**
   * number値(double)のget
   * @return number値(double)
   */
  double GetNumberDouble() const override
  {
    return static_cast<double>(m_value);
  }

  /**
   * JSON形式の文字列を出力
   * @param[in,out] outStream 出力先のストリーム
   */
  void Dump(std::ostream* outStream) const override
  {
    *outStream << m_value << std::flush;
  }

  /**
   * MessagePack形式のバイナリを出力
   * @param[in,out] outStream 出力先のストリーム
   */
  void DumpMsgpack(std::ostream* outStream) const override
  {
    msgpack::pack(outStream, m_value);
  }
};
}
#endif // ANALYSISDRIVE_JSONVALUEINT_H_
