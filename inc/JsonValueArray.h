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
#include "IncludeMsgpack.h"
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

  /**
   * JSON形式の文字列を出力
   * @param[in,out] outStream 出力先のストリーム
   */
  void Dump(std::ostream* outStream) const override
  {
    // 前括弧
    *outStream << "[";
    // 要素がある場合のみ出力処理を行う
    if (m_value.size() > 0)
    {
      // イテレータ取得
      auto it = m_value.begin();
      // 要素数ループ
      while (true)
      {
        // 出力
        it->Dump(outStream);
        // イテレータを進める
        it++;
        // 終わりでない場合
        if (it != m_value.end())
        {
          // カンマを出力
          *outStream << ",";
        }
        // 終わりの場合
        else
        {
          break;
        }
      }
    }
    // 後括弧
    *outStream << "]" << std::flush;
  }

  /**
   * MessagePack形式のバイナリを出力
   * @param[in,out] outStream 出力先のストリーム
   */
  void DumpMsgpack(std::ostream* outStream) const override
  {
    msgpack::packer<std::ostream> pk(outStream);
    // arrayサイズを指定
    pk.pack_array(m_value.size());
    // 要素数出力
    for (auto it = m_value.begin(); it != m_value.end(); ++it)
    {
      // 出力
      it->DumpMsgpack(outStream);
    }
  }
};
}
#endif // ANALYSISDRIVE_JSONVALUEARRAY_H_
