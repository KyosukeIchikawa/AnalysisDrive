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
#include "IncludeMsgpack.h"
#include <string>
#include <unordered_map>

/** Analysis Drive */
namespace adlib
{
/** JSONの値(object) */
class JsonValueObject final : public JsonValue
{
private:
  //! 値
  Json::Object m_value;

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
  explicit JsonValueObject(const Json::Object& value) : m_value(value) {}

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
  Json::Object& GetObject() override
  {
    return m_value;
  }

  /**
   * object値のget(const版)
   * @return object値
   */
  const Json::Object& GetObject() const override
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
    *outStream << "{";
    // 要素がある場合のみ出力処理を行う
    if (m_value.size() > 0)
    {
      // イテレータ取得
      auto it = m_value.begin();
      // 要素数ループ
      while (true)
      {
        // nameを出力
        *outStream << "\"" << it->first << "\"" << ":";
        // 出力
        it->second.Dump(outStream);
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
    *outStream << "}" << std::flush;
  }

  /**
   * MessagePack形式のバイナリを出力
   * @param[in,out] outStream 出力先のストリーム
   */
  void DumpMsgpack(std::ostream* outStream) const override
  {
    msgpack::packer<std::ostream> pk(outStream);
    // objectサイズを指定
    pk.pack_map(m_value.size());
    // 要素数出力
    for (auto it = m_value.begin(); it != m_value.end(); ++it)
    {
      // nameを出力
      pk.pack(it->first);
      // valueを出力
      it->second.DumpMsgpack(outStream);
    }
  }
};
}
#endif // ANALYSISDRIVE_JSONVALUEOBJECT_H_
