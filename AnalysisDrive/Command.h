/**
 * @file   Command.h
 * @brief  コマンドオブジェクト
 * @author kyo
 * @date   2015/05/01
 */
#ifndef ANALYSISDRIVE_COMMAND_H_
#define ANALYSISDRIVE_COMMAND_H_

#include <memory>
#include <vector>
#include <string>

/** Analysis Drive */
namespace  adlib
{
/** コマンドオブジェクト */
class Command
{
private:
  //! Pimplイディオム
  class Impl;
  std::unique_ptr<Impl> m_impl;

private:
  /** コピー禁止 */
  Command(const Command&);
  /** 代入禁止 */
  void operator=(const Command&);

public:
  /** コンストラクタ */
  Command();

  /**
   * コンストラクタ
   * @param[in] prefix    接頭文字
   * @param[in] arguments 引数
   */
  Command(const std::string& prefix, const std::vector<std::string>& arguments);

  /**
   * ムーブコンストラクタ
   * @param[in] obj ムーブオブジェクト
   */
  Command(Command&& obj);

  /** 仮想デストラクタ */
  virtual ~Command();

  /**
   * 接頭文字をget
   * @return 接頭文字
   * @remarks 空文字の場合は接頭文字なし
   */
  const std::string& Prefix() const;

  /**
   * 引数が格納されたコンテナの参照をget
   * @return 引数参照
   */
  const std::vector<std::string>& Args() const;
};
}
#endif // ANALYSISDRIVE_COMMAND_H_
