/**
 * @file   CommandLineSetting.cpp
 * @brief  コマンドライン読み込み設定
 * @author kyo
 * @date   2015/05/01
 */
#include "CommandLineSetting.h"
#include <memory>
#include <string>

/** Analysis Drive */
namespace adlib
{
//! Pimplイディオム
class CommandLineSetting::Impl
{
public:
  //! 接頭文字
  std::string m_prefix;
  //! 接頭文字後続の引数の数
  int m_numParameter;
  //! ヘルプ表記
  std::string m_help;

  /**
   * コンストラクタ
   * @param[in] prefix       接頭文字
   * @param[in] numParameter 接頭文字後続の引数の数
   * @param[in] help         ヘルプ表記
   */
  Impl(const std::string& prefix, int numParameter, const std::string& help)
    :m_prefix(prefix),
     m_numParameter(numParameter),
     m_help(help)
  {
  }

  /**
   * コピーコンストラクタ
   * @param[in] obj コピー元
   */
  Impl(const Impl& obj)
    :m_prefix(obj.m_prefix),
     m_numParameter(obj.m_numParameter),
     m_help(obj.m_help)
  {
  }

  /**
   * 代入
   * @param[in] obj 代入オブジェクト
   * @return 代入後の自身
   */
  Impl& operator=(const Impl& obj)
  {
    m_prefix = obj.m_prefix;
    m_numParameter = obj.m_numParameter;
    m_help = obj.m_help;
    return *this;
  }
};

/**
 * コンストラクタ
 * @param[in] prefix       接頭文字
 * @param[in] numParameter 接頭文字後続の引数の数
 * @param[in] help         ヘルプ表記
 */
CommandLineSetting::CommandLineSetting(const std::string& prefix, int numParameter, const std::string& help)
  :m_impl(std::make_unique<Impl>(prefix, numParameter, help))
{
}

/*
 * コピーコンストラクタ
 * @param[in] obj コピー元
 */
CommandLineSetting::CommandLineSetting(const CommandLineSetting& obj)
  :m_impl(std::make_unique<Impl>(obj.m_impl->m_prefix, obj.m_impl->m_numParameter, obj.m_impl->m_help))
{
}

/** デストラクタ */
CommandLineSetting::~CommandLineSetting() {}

/**
 * 代入
 * @param[in] obj 代入オブジェクト
 * @return 代入後の自身
 */
CommandLineSetting& CommandLineSetting::operator=(const CommandLineSetting& obj)
{
  *(this->m_impl) = *(obj.m_impl);
  return *this;
}

/**
 * 接頭文字をget
 * @return 接頭文字
 */
const std::string& CommandLineSetting::Prefix() const
{
  return m_impl->m_prefix;
}

/**
 * 接頭文字後続の引数の数をget
 * @return 引数の数
 */
int CommandLineSetting::NumberOfParameter() const
{
  return m_impl->m_numParameter;
}

/**
 * ヘルプ表記をget
 * @return ヘルプ表記
 */
const std::string& CommandLineSetting::Help() const
{
  return m_impl->m_help;
}

/**
 * 接頭文字が不一致かどうか
 * @param[in] obj 比較対象
 * @return 不一致であればtrue
 */
bool CommandLineSetting::operator!=(const CommandLineSetting& obj) const
{
  return m_impl->m_prefix != obj.m_impl->m_prefix;
}

/**
 * 接頭文字が一致かどうか
 * @param[in] obj 比較対象
 * @return 一致であればtrue
 */
bool CommandLineSetting::operator==(const CommandLineSetting& obj) const
{
  return !(this->operator!=(obj));
}

/**
 * ヘルプを表示
 * @param[in]     settings     コマンドライン読み込み設定コンテナ
 * @param[in]     noPrefixHelp 接頭文字なしの引数に関するヘルプ表示(接頭文字なしの引数が不要であれば空文字"")
 * @param[in,out] outStream    出力先のストリーム
 */
void CommandLineSetting::ShowHelp(
  const std::vector<CommandLineSetting>& settings,
  const std::string& noPrefixHelp,
  std::ostream* outStream)
{
  //----- パラメータの説明 -----//
  // 設定数ループ
  for (auto setting = settings.begin(); setting != settings.end(); ++setting)
  {
    // 接頭文字表示
    *outStream << setting->Prefix() << std::flush;
    // 後続のパラメータが必要な場合は数を表示
    if (setting->NumberOfParameter() > 0)
    {
      *outStream << " (+ " << setting->NumberOfParameter() << " parameters)" << std::flush;
    }
    *outStream << std::endl;
    // ヘルプ表示
    *outStream << setting->Help() << std::endl;
    // 改行
    *outStream << std::endl;
  }
  // 接頭文字なしの引数に関するヘルプ表示
  if (noPrefixHelp != "")
  {
    *outStream << "NO PREFIX" << std::endl;
    // ヘルプ表示
    *outStream << noPrefixHelp << std::endl;
    // 改行
    *outStream << std::endl;
  }
}
}
