/**
 * @file   JsonValueNull.h
 * @brief  JSONの値(null)
 * @author kyo
 * @date   2015/05/05
 */
#ifndef ANALYSISDRIVE_JSONVALUENULL_H_
#define ANALYSISDRIVE_JSONVALUENULL_H_

#include "JsonValue.h"
#include "Json.h"
#include "IncludeMsgpack.h"

/** Analysis Drive */
namespace adlib
{
/** JSONの値(bool) */
class JsonValueNull final : public JsonValue
{
private:
  /** デフォルトコンストラクタ禁止 */
  JsonValueNull() {}
  /** コピー禁止 */
  JsonValueNull(const JsonValueNull&);
  /** 代入禁止 */
  void operator=(const JsonValueNull&);

public:
  /**
   * コンストラクタ
   * @tparam value nullptr
   */
  explicit JsonValueNull(std::nullptr_t value) {}

  /** 仮想デストラクタ */
  virtual ~JsonValueNull() {}

  /**
   * 型を取得
   * @return 型
   */
  Json::JSON_VALUE_TYPE GetValueType() const override
  {
    return Json::JSON_VALUE_TYPE::NUL;
  }

  /**
   * JSON形式の文字列を出力
   * @param[in,out] outStream 出力先のストリーム
   */
  void Dump(std::ostream* outStream) const override
  {
    *outStream << "null" << std::flush;
  }

  /**
   * MessagePack形式のバイナリを出力
   * @param[in,out] outStream 出力先のストリーム
   */
  void DumpMsgpack(std::ostream* outStream) const override
  {
    msgpack::pack(outStream, msgpack::type::nil());
  }
};
}
#endif // ANALYSISDRIVE_JSONVALUENULL_H_
