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
};
}
#endif // ANALYSISDRIVE_JSONVALUENULL_H_
