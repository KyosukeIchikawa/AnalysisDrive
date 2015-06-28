/**
 * @file   MsgpackParser.cpp
 * @brief  MessagePackパーサー
 * @author kyo
 * @date   2015/06/29
 */
#include "MsgpackParser.h"
#include "Json.h"
#include "IncludeMsgpack.h"
#include <memory>
#include <vector>
#include <map>
#include <string>

/** Analysis Drive */
namespace adlib
{
//> Pimplイディオム
class MsgpackParser::Impl
{
public:
  //> エラーメッセージを格納するバッファサイズ
  static const size_t BUFFER_SIZE;
  //> エラーメッセージ --- 予期せぬエラー
  static const std::string ERR_MSG_UNKNOWN;
  //> ワーニングメッセージ --- mapのnameが重複している
  static const std::string WRN_MSG_NAME_REPEATED;
  //> ワーニングメッセージ --- 未対応の型(Binary型)を検出
  static const std::string WRN_MSG_BINARY_TYPE_EXIST;
  //> ワーニングメッセージ --- 未対応の型(Extension型)を検出
  static const std::string WRN_MSG_EXTENSION_TYPE_EXIST;
  //> ワーニングメッセージ --- 未対応の型を検出
  static const std::string WRN_MSG_UNKNOWN_TYPE_EXIST;

  //> エラーメッセージコンテナ
  std::vector<std::string> m_errorMsg;
  //> ワーニングメッセージコンテナ
  std::vector<std::string> m_warningMsg;
  //> Mapのname重複フラグ
  bool m_nameRepeated;

  /**
   * コンストラクタ
   */
  Impl()
    :m_nameRepeated(false)
  {
  }

  /**
   * エラーメッセージを作成する
   * @param[in] messageTemp エラーメッセージ
   * @param[in] keyWord     メッセージに埋め込むキーワード
   * @return エラーメッセージ
   * @remarks messageTempの「%s」の箇所にkeyWordを埋め込んで返す
   */
  const std::string MakeErrorMsg(const std::string& messageTemp, const std::string& keyWord) const;

  /**
   * string型にパースする
   * @param[in] msgobj MessagePackオブジェクト
   */
  Json ParseString(const msgpack::object& msgobj);

  /**
   * number型(整数)にパースする
   * @param[in] msgobj MessagePackオブジェクト
   */
  Json ParseNumberInt(const msgpack::object& msgobj);

  /**
   * number型(浮動小数点数)にパースする
   * @param[in] msgobj MessagePackオブジェクト
   */
  Json ParseNumberDouble(const msgpack::object& msgobj);

  /**
   * object型にパースする
   * @param[in] msgobj MessagePackオブジェクト
   */
  Json ParseObject(const msgpack::object& msgobj);

  /**
   * array型にパースする
   * @param[in] msgobj MessagePackオブジェクト
   */
  Json ParseArray(const msgpack::object& msgobj);

  /**
   * valueにパースする
   * @param[in] msgobj MessagePackオブジェクト
   */
  Json ParseValue(const msgpack::object& msgobj);
};

//> エラーメッセージを格納するバッファサイズ
const size_t MsgpackParser::Impl::BUFFER_SIZE = 256;
//> エラーメッセージ --- 予期せぬエラー
const std::string MsgpackParser::Impl::ERR_MSG_UNKNOWN("Unknown Error.");
//> ワーニングメッセージ --- mapのnameが重複している
const std::string MsgpackParser::Impl::WRN_MSG_NAME_REPEATED("Name \"%s\" Repeated, and Later Name is Given Priority.");
//> ワーニングメッセージ --- 未対応の型(Binary型)を検出
const std::string MsgpackParser::Impl::WRN_MSG_BINARY_TYPE_EXIST("Binary Type is not Supported, and null is Given Value.");
//> ワーニングメッセージ --- 未対応の型(Extension型)を検出
const std::string MsgpackParser::Impl::WRN_MSG_EXTENSION_TYPE_EXIST("Extension Type is not Supported, and null is Given Value.");
//> ワーニングメッセージ --- 未対応の型を検出
const std::string MsgpackParser::Impl::WRN_MSG_UNKNOWN_TYPE_EXIST("Unknown Type is Detected, and null is Given Value.");

/**
 * エラーメッセージを作成する
 * @param[in] messageTemp エラーメッセージ
 * @param[in] keyWord     メッセージに埋め込むキーワード
 * @return エラーメッセージ
 * @remarks messageTempの「%s」の箇所にkeyWordを埋め込んで返す
 */
const std::string MsgpackParser::Impl::MakeErrorMsg(const std::string& messageTemp, const std::string& keyWord) const
{
  // バッファを用意
  char buf[Impl::BUFFER_SIZE];
  // エラーメッセージを作成して返す
  sprintf_s(buf, sizeof(buf), messageTemp.c_str(), keyWord.c_str());
  return std::string(buf);
}

/**
 * string型にパースする
 * @param[in] msgobj MessagePackオブジェクト
 */
Json MsgpackParser::Impl::ParseString(const msgpack::object& msgobj)
{
  // string型にコンバートする
  std::string str;
  msgobj.convert(&str);
  // string型Jsonオブジェクトを返す
  return Json(str);
}

/**
 * number型(整数)にパースする
 * @param[in] msgobj MessagePackオブジェクト
 */
Json MsgpackParser::Impl::ParseNumberInt(const msgpack::object& msgobj)
{
  // int型にコンバートする
  int num;
  msgobj.convert(&num);
  // int型Jsonオブジェクトを返す
  return Json(num);
}

/**
 * number型(浮動小数点数)にパースする
 * @param[in] msgobj MessagePackオブジェクト
 */
Json MsgpackParser::Impl::ParseNumberDouble(const msgpack::object& msgobj)
{
  // double型にコンバートする
  double num;
  msgobj.convert(&num);
  // double型Jsonオブジェクトを返す
  return Json(num);
}

/**
 * object型にパースする
 * @param[in] msgobj MessagePackオブジェクト
 */
Json MsgpackParser::Impl::ParseObject(const msgpack::object& msgobj)
{
  // バッファを用意
  std::map<std::string, Json> obj;

  // 要素があれば格納する
  if (msgobj.via.map.size > 0U)
  {
    //
    // 下記の処理は本家MessagePackのコードobject.cを参考にした
    //

    // object_kv構造体を作成
    msgpack::object_kv* p = msgobj.via.map.ptr;
    // 終端を設定
    msgpack::object_kv* const pend = msgobj.via.map.ptr + msgobj.via.map.size;
    // 終端までループ(要素数ループ)
    for (; p < pend; ++p)
    {
      // キーを取得
      std::string key;
      p->key.convert(&key);

      // キーが重複している場合
      if (obj.count(key) > 0)
      {
        // 重複フラグをたてる
        m_nameRepeated = true;
        // ワーニングメッセージ格納
        m_warningMsg.emplace_back(
          MakeErrorMsg(Impl::WRN_MSG_NAME_REPEATED, key));
      }

      // 値を格納
      obj[key] = this->ParseValue(p->val);
    }
  }
  // object型Jsonオブジェクトを返す
  return Json(obj);
}

/**
 * array型にパースする
 * @param[in] msgobj MessagePackオブジェクト
 */
Json MsgpackParser::Impl::ParseArray(const msgpack::object& msgobj)
{
  // バッファを用意
  std::vector<Json> arr;

  // 要素があれば格納する
  if (msgobj.via.array.size > 0U)
  {
    //
    // 下記の処理は本家MessagePackのコードobject.cを参考にした
    //

    // msgpack_object構造体を作成
    msgpack::object* p = msgobj.via.array.ptr;
    // 終端を設定
    msgpack::object* const pend = msgobj.via.array.ptr + msgobj.via.array.size;
    // 終端までループ(要素数ループ)
    for (; p < pend; ++p)
    {
      // 値を格納
      arr.emplace_back(this->ParseValue(*p));
    }
  }
  // array型Jsonオブジェクトを返す
  return Json(arr);
}

/**
 * valueにパースする
 * @param[in] msgobj MessagePackオブジェクト
 */
Json MsgpackParser::Impl::ParseValue(const msgpack::object& msgobj)
{
  // objectタイプ分岐
  switch (msgobj.type)
  {
    case msgpack::type::object_type::BOOLEAN:
      return Json(msgobj.via.boolean);

    case msgpack::type::object_type::FLOAT:
      return this->ParseNumberDouble(msgobj);

    case msgpack::type::object_type::POSITIVE_INTEGER:
    case msgpack::type::object_type::NEGATIVE_INTEGER:
      return this->ParseNumberInt(msgobj);

    case msgpack::type::object_type::STR:
      return this->ParseString(msgobj);

    case msgpack::type::object_type::NIL:
      return Json(nullptr);

    case msgpack::type::object_type::ARRAY:
      return this->ParseArray(msgobj);

    case msgpack::type::object_type::MAP:
      return this->ParseObject(msgobj);

    case msgpack::type::object_type::BIN:
      // Binary型は未対応なので, ワーニングメッセージ格納
      m_warningMsg.emplace_back(Impl::WRN_MSG_BINARY_TYPE_EXIST);
      // 一応nullとしておく
      return Json(nullptr);

    case msgpack::type::object_type::EXT:
      // Extension型は未対応なので, ワーニングメッセージ格納
      m_warningMsg.emplace_back(Impl::WRN_MSG_EXTENSION_TYPE_EXIST);
      // 一応nullとしておく
      return Json(nullptr);

    default:
      // 未対応の型なので, ワーニングメッセージ格納
      m_warningMsg.emplace_back(Impl::WRN_MSG_UNKNOWN_TYPE_EXIST);
      // 一応nullとしておく
      return Json(nullptr);
  }
}

/** コンストラクタ */
MsgpackParser::MsgpackParser()
  : m_impl(std::make_unique<Impl>())
{
}

/** デストラクタ */
MsgpackParser::~MsgpackParser() {}

/**
 * MessagePack形式の文字列にパースする
 * @param[in] target パース対象の文字列
 * @return Jsonオブジェクト
 * @attention これまでのエラーメッセージやワーニングメッセージが初期化される
 */
Json MsgpackParser::Parse(const std::string& target)
{
  // 前回のパース結果を初期化
  m_impl = std::make_unique<Impl>();

  // デシリアライズ
  msgpack::unpacked result;
  msgpack::unpack(result, target.data(), target.size());
  const msgpack::object& msgobj = result.get();

  try
  {
    // パースする
    return m_impl->ParseValue(msgobj);
  }
  catch (const std::string& err)
  {
    // エラーメッセージ格納
    m_impl->m_errorMsg.emplace_back(err);
    // 失敗時はnull型Jsonオブジェクトを返す
    return Json(nullptr);
  }
  catch (...) // 予期せぬエラー(MessagePack関連のエラーなど)
  {
    // エラーメッセージ格納
    m_impl->m_errorMsg.emplace_back(Impl::ERR_MSG_UNKNOWN);
    // 失敗時はnull型Jsonオブジェクトを返す
    return Json(nullptr);
  }
}

/**
 * エラーメッセージを取得する
 * @return エラーメッセージのコンテナ(正常であればサイズ0)
 */
const std::vector<std::string>& MsgpackParser::ErrorMessage() const
{
  return m_impl->m_errorMsg;
}

/**
 * ワーニングメッセージを取得する
 * @return ワーニングメッセージのコンテナ(正常であればサイズ0)
 */
const std::vector<std::string>& MsgpackParser::WarningMessage() const
{
  return m_impl->m_warningMsg;
}

/**
 * Mapのnameが重複していればtrueを返す
 * @return 前回のParseでMapのnameが重複していればtrueを返す
 */
bool MsgpackParser::IsNameRepeated() const
{
  return m_impl->m_nameRepeated;
}
}
