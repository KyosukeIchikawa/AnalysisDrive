/**
 * @file   CommandLineSetting.h
 * @brief  コマンドライン読み込み設定
 * @author kyo
 * @date   2015/05/01
 */
#ifndef ANALYSISDRIVE_COMMANDLINESETTING_H_
#define ANALYSISDRIVE_COMMANDLINESETTING_H_

#include <memory>
#include <string>
#include <vector>

/** Analysis Drive */
namespace  adlib
{
/** コマンドライン読み込み設定 */
class CommandLineSetting final
{
private:
  //! Pimplイディオム
  class Impl;
  std::unique_ptr<Impl> m_impl;

private:
  /** デフォルトコンストラクタ禁止 */
  CommandLineSetting();

public:
  /**
   * コンストラクタ
   * @param[in] prefix       接頭文字(空文字は禁止)
   * @param[in] numParameter 接頭文字後続の引数の数
   * @param[in] help         ヘルプ表記
   */
  CommandLineSetting(const std::string& prefix, int numParameter, const std::string& help);

  /*
   * コピーコンストラクタ
   * @param[in] obj コピー元
   */
  CommandLineSetting(const CommandLineSetting& obj);

  /** 仮想デストラクタ */
  virtual ~CommandLineSetting();

  /**
   * 代入
   * @param[in] obj 代入オブジェクト
   * @return 代入後の自身
   */
  CommandLineSetting& operator=(const CommandLineSetting& obj);

  /**
   * 接頭文字をget
   * @return 接頭文字
   */
  const std::string& Prefix() const;

  /**
   * 接頭文字後続の引数の数をget
   * @return 引数の数
   */
  int NumberOfParameter() const;

  /**
   * ヘルプ表記をget
   * @return ヘルプ表記
   */
  const std::string& Help() const;

  /**
   * 接頭文字が不一致かどうか
   * @param[in] obj 比較対象
   * @return 不一致であればtrue
   */
  bool operator!=(const CommandLineSetting& obj) const;

  /**
   * 接頭文字が一致かどうか
   * @param[in] obj 比較対象
   * @return 一致であればtrue
   */
  bool operator==(const CommandLineSetting& obj) const;

  /**
   * ヘルプを表示
   * @param[in]     settings     コマンドライン読み込み設定コンテナ
   * @param[in]     noPrefixHelp 接頭文字なしの引数に関するヘルプ表示(接頭文字なしの引数が不要であれば空文字"")
   * @param[in,out] outStream    出力先のストリーム
   */
  static void ShowHelp(
    const std::vector<CommandLineSetting>& settings,
    const std::string& noPrefixHelp,
    std::ostream* outStream);
};
}
#endif // ANALYSISDRIVE_COMMANDLINESETTING_H_
