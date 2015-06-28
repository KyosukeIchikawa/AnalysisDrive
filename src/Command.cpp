/**
 * @file   Command.cpp
 * @brief  コマンドオブジェクト
 * @author kyo
 * @date   2015/05/01
 */
#include "Command.h"
#include <memory>
#include <string>
#include <vector>

/** Analysis Drive */
namespace adlib
{
//! Pimplイディオム
class Command::Impl
{
public:
  //! 接頭文字
  const std::string m_prefix;
  //! 引数
  const std::vector<std::string> m_arguments;

  /**
   * コンストラクタ
   * @param[in] prefix    接頭文字
   * @param[in] arguments 引数
   */
  Impl(const std::string& prefix, const std::vector<std::string>& arguments)
    :m_prefix(prefix),
     m_arguments(arguments)
  {
  }

  /**
   * ムーブコンストラクタ
   * @param[in] obj ムーブオブジェクト
   */
  Impl(Impl&& obj)
    :m_prefix(std::move(obj.m_prefix)),
     m_arguments(std::move(obj.m_arguments))
  {
  }
};

/** コンストラクタ */
Command::Command()
  :m_impl(std::make_unique<Impl>("", std::vector<std::string>()))
{
}

/**
 * コンストラクタ
 * @param[in] prefix    接頭文字
 * @param[in] arguments 引数
 */
Command::Command(const std::string& prefix, const std::vector<std::string>& arguments)
  :m_impl(std::make_unique<Impl>(prefix, arguments))
{
}

/**
 * ムーブコンストラクタ
 * @param[in] obj ムーブオブジェクト
 */
Command::Command(Command&& obj)
  :m_impl(std::move(m_impl))
{
}

/** デストラクタ */
Command::~Command() {}

/**
 * 接頭文字をget
 * @return 接頭文字
 */
const std::string& Command::Prefix() const
{
  return m_impl->m_prefix;
}

/**
 * 引数が格納されたコンテナの参照をget
 * @return 引数参照
 */
const std::vector<std::string>& Command::Args() const
{
  return m_impl->m_arguments;
}
}
