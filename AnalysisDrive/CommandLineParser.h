/**
 * @file   CommandLineParser.h
 * @brief  コマンドライン読み込み機能
 * @author kyo
 * @date   2015/05/01
 */
#ifndef ANALYSISDRIVE_COMMANDLINEPARSER_H_
#define ANALYSISDRIVE_COMMANDLINEPARSER_H_

#include "CommandLineSetting.h"
#include "Command.h"
#include <memory>
#include <string>
#include <unordered_map>

/** Analysis Drive */
namespace  adlib
{
/** コマンドライン読み込み機能 */
class CommandLineParser final
{
private:
  //! Pimplイディオム
  class Impl;
  std::unique_ptr<Impl> m_impl;

private:
  /** コピー禁止 */
  CommandLineParser(const CommandLineParser&);
  /** 代入禁止 */
  void operator=(const CommandLineParser&);

public:
  /** コンストラクタ */
  CommandLineParser();

  /** 仮想デストラクタ */
  virtual ~CommandLineParser();

  /**
   * コマンドライン引数をパースする
   * @param[in] settings コマンドライン読み込み設定コンテナ
   * @param[in] argc     argvの配列サイズ
   * @param[in] argv     コマンドライン引数
   * @return コマンドオブジェクトの連想配列
   * @attention これまでのエラーメッセージやワーニングメッセージが初期化される
   */
  const std::unordered_map<std::string, Command> Parse(
    const std::vector<CommandLineSetting>& settings,
    int argc,
    const char* const argv[]);

  /**
   * 前回Parseで読んだコマンド内容を文字列でget
   * @return コマンド内容(exeを含む)
   */
  const std::string& GetCommandArguments() const;

  /**
   * エラーメッセージを取得する
   * @return エラーメッセージのコンテナ(正常であればサイズ0)
   */
  const std::vector<std::string>& ErrorMessage() const;
};
}
#endif // ANALYSISDRIVE_COMMANDLINEPARSER_H_
