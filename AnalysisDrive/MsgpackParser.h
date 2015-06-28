/**
 * @file   MsgpackParser.h
 * @brief  MessagePackパーサー
 * @author kyo
 * @date   2015/06/29
 */
#ifndef ANALYSISDRIVE_MSGPACKPARSER_H_
#define ANALYSISDRIVE_MSGPACKPARSER_H_

#include <memory>
#include <string>
#include <vector>

/** Analysis Drive */
namespace adlib
{
// 前方宣言
class Json;

/**
 * MessagePackパーサー
 */
class MsgpackParser
{
private:
  //> Pimplイディオム
  class Impl;
  std::unique_ptr<Impl> m_impl;

private:
  /* コピー禁止 */
  MsgpackParser(const MsgpackParser&);
  /* 代入禁止 */
  void operator=(const MsgpackParser&);

public:
  /** コンストラクタ */
  MsgpackParser();
  /** 仮想デストラクタ */
  virtual ~MsgpackParser();

  /**
   * MessgaePack形式の文字列をパースする
   * @param[in] target パース対象の文字列
   * @return Jsonオブジェクト
   * @attention これまでのエラーメッセージやワーニングメッセージが初期化される
   */
  Json Parse(const std::string& target);

  /**
   * エラーメッセージを取得する
   * @return エラーメッセージのコンテナ(正常であればサイズ0)
   */
  const std::vector<std::string>& ErrorMessage() const;

  /**
   * ワーニングメッセージを取得する
   * @return ワーニングメッセージのコンテナ(正常であればサイズ0)
   */
  const std::vector<std::string>& WarningMessage() const;

  /**
   * Mapのnameが重複していればtrueを返す
   * @return 前回のParseでMapのnameが重複していればtrueを返す
   */
  bool IsNameRepeated() const;
};
}
#endif // ANALYSISDRIVE_MSGPACKPARSER_H_
