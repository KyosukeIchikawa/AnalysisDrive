/**
 * @file   Json.h
 * @brief  JSONオブジェクト
 * @author kyo
 * @date   2015/05/01
 */
#ifndef ANALYSISDRIVE_JSON_H_
#define ANALYSISDRIVE_JSON_H_

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <ostream>

/** Analysis Drive */
namespace adlib
{
/** JSONオブジェクト */
class Json final
{
public:
  //! JSONの型
  enum JSON_VALUE_TYPE
  {
    NUL,           /*!< null           */
    STRING,        /*!< string         */
    NUMBER_INT,    /*!< number(int)    */
    NUMBER_DOUBLE, /*!< number(double) */
    BOOL,          /*!< bool           */
    ARRAY,         /*!< array          */
    OBJECT         /*!< object         */
  };

  //! JSONオブジェクトでのarray型定義
  typedef std::vector<Json> Array;
  //! JSONオブジェクトでのobject型定義
  typedef std::unordered_map<std::string, Json> Object;

private:
  //! Pimplイディオム
  class Impl;
  std::unique_ptr<Impl> m_impl;

public:
  /**
   * コンストラクタ
   * @remarks 引数なしのコンストラクタではnullのJSONオブジェクトを作成する
   * @note Json::Objectを存在しないキーでアクセスした場合, <br>
   *       本コンストラクタによってJsonが新規登録される. <br>
   *       これにより, 存在しないキーでアクセスするとnullと同様の値になる.
   */
  Json();

  /**
   * コンストラクタ
   * @param[in] value 値(null)
   */
  explicit Json(std::nullptr_t value);

  /**
   * コンストラクタ
   * @param[in] value 値(string型)
   */
  explicit Json(const std::string& value);

  /**
   * コンストラクタ
   * @param[in] value 値(number型(int))
   */
  explicit Json(int value);

  /**
   * コンストラクタ
   * @param[in] value 値(number型(double))
   */
  explicit Json(double value);

  /**
   * コンストラクタ
   * @param[in] value 値(bool型)
   */
  explicit Json(bool value);

  /**
   * コンストラクタ
   * @param[in] value 値(array型)
   */
  explicit Json(const Json::Array& value);

  /**
   * コンストラクタ
   * @param[in] value 値(object型)
   */
  explicit Json(const Json::Object& value);

  /**
   * コピーコンストラクタ
   * @param[in] obj コピー元
   */
  Json(const Json& obj);

  /**
   * ムーブコンストラクタ
   * @param[in] obj ムーブオブジェクト
   */
  Json(Json&& obj);

  /**
   * 代入
   * @param[in] obj 代入するオブジェクト
   */
  Json& operator=(const Json& obj);

  /** 仮想デストラクタ */
  virtual ~Json();

  /**
   * 型を取得
   * @return 型
   */
  JSON_VALUE_TYPE GetValueType() const;

  /**
   * nullかどうか
   * @return nullならtrue
   */
  bool IsNull() const;

  /**
   * stringかどうか
   * @return stringならtrue
   */
  bool IsString() const;

  /**
   * number(int or double)かどうか
   * @return numberならtrue
   */
  bool IsNumber() const;

  /**
   * number(int)かどうか
   * @return number(int)ならtrue
   */
  bool IsNumberInt() const;

  /**
   * number(double)かどうか
   * @return number(double)ならtrue
   */
  bool IsNumberDouble() const;

  /**
   * boolかどうか
   * @return boolならtrue
   */
  bool IsBool() const;

  /**
   * arrayかどうか
   * @return arrayならtrue
   */
  bool IsArray() const;

  /**
   * objectかどうか
   * @return objectならtrue
   */
  bool IsObject() const;

  /**
   * string値のget
   * @return string値
   */
  const std::string& GetString() const;

  /**
   * number値(int)のget
   * @return number値(int)
   */
  int GetNumberInt() const;

  /**
   * number(double)のget
   * @return number値(double)
   */
  double GetNumberDouble() const;

  /**
   * bool値のget
   * @return bool値
   */
  bool GetBool() const;

  /**
   * array値のget
   * @return array値
   */
  Json::Array& GetArray();

  /**
   * array値のget(const版)
   * @return array値
   */
  const Json::Array& GetArray() const;

  /**
   * object値のget
   * @return object値
   */
  Json::Object& GetObject();

  /**
   * object値のget(const版)
   * @return object値
   */
  const Json::Object& GetObject() const;

  /**
   * JSON形式の文字列を出力
   * @param[in,out] outStream 出力先のストリーム
   */
  void Dump(std::ostream* outStream) const;

  /**
   * MessagePack形式のバイナリを出力
   * @param[in,out] outStream 出力先のストリーム
   */
  void DumpMsgpack(std::ostream* outStream) const;
};
}
#endif // ANALYSISDRIVE_JSON_H_
