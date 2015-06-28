/**
 * @file   CommandLineParser.cpp
 * @brief  コマンドライン読み込み機能
 * @author kyo
 * @date   2015/05/01
 */
#include "CommandLineParser.h"
#include "CommandLineSetting.h"
#include "Command.h"
#include <memory>
#include <vector>
#include <string>
#include <map>

/** Analysis Drive */
namespace adlib
{
//! Pimplイディオム
class CommandLineParser::Impl
{
public:
  //! セラーメッセージを格納するバッファサイズ
  static const size_t BUFFER_SIZE;
  //! エラーメッセージ --- 接頭辞が重複して設定されている(ライブラリ利用者のエラー)
  static const std::string ERR_MSG_SETTING_DOUBLE_PREFIX;
  //! エラーメッセージ --- コマンドライン引数の内容がない(実行ファイル名もない. ライブラリ利用者のエラー)
  static const std::string ERR_MSG_NOT_EXIST_COMMANDLINE_CONTENTS;
  //! エラーメッセージ --- 空白の接頭辞がある(ライブラリ利用者のエラー)
  static const std::string ERR_MSG_EXIST_NOTHING_PREFIX_IN_SETTING;
  //! エラーメッセージ --- 接頭辞が重複して入力されている
  static const std::string ERR_MSG_DOUBLE_PREFIX_INPUT;
  //! エラーメッセージ --- パラメータが不足している
  static const std::string ERR_MSG_PARAMETER_INSUFFICIENT;
  //! エラーメッセージ --- 無効なパラメータが入力されている
  static const std::string ERR_MSG_PARAMETER_INVALID;

  //! エラーメッセージコンテナ
  std::vector<std::string> m_errorMsg;
  //! コマンド内容
  std::string m_arguments;

  //! コンストラクタ
  Impl()
    :m_arguments("")
  {
  }

  /**
   * 代入演算子
   * @param[in] obj 代入オブジェクト
   */
  Impl& operator=(const Impl& obj)
  {
    m_errorMsg = obj.m_errorMsg;
    m_arguments = obj.m_arguments;
    return *this;
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
   * 設定された接頭辞のエラーチェックをする(エラーがある場合は例外を投げる)
   * @param[in] settings コマンドライン読み込み設定コンテナ
   * @remarks ・空白の接頭辞があればエラー <br>
   *          ・重複があればエラー
   */
  void CheckSettingPrefix(const std::vector<CommandLineSetting>& settings) const;

  /**
   * コマンド内容を記録しておく
   * @param[in] argc argvの配列サイズ
   * @param[in] argv コマンドライン引数
   */
  void SaveArguments(int argc, const char* const argv[]);

  /**
   * 入力されたコマンドに接頭辞の重複がないかチェックする(重複がある場合は例外を投げる)
   * @param[in] settings コマンドライン読み込み設定コンテナ
   * @param[in] argc argvの配列サイズ
   * @param[in] argv コマンドライン引数
   * @return 最も後ろの接頭文字の影響範囲(接頭文字のインデックス + 後ろに取る引数の数)
   */
  int CheckDoublePrefixInput(
    const std::vector<CommandLineSetting>& settings,
    int argc,
    const char* const argv[]) const;

  /**
   * コマンドオブジェクトの連想配列を作成する
   * @param[in] settings            コマンドライン読み込み設定コンテナ
   * @param[in] argc                argvの配列サイズ
   * @param[in] argv                コマンドライン引数
   * @param[in] indexLastPrefixArea 最も後ろの接頭文字の影響範囲
   */
  const std::map<std::string, Command> MakeCommandArray(
    const std::vector<CommandLineSetting>& settings,
    int argc,
    const char* const argv[],
    int indexLastPrefixArea) const;
};

//! エラーメッセージを格納するバッファサイズ
const size_t CommandLineParser::Impl::BUFFER_SIZE = 256;
//! エラーメッセージ --- 接頭辞が重複して設定されている(ライブラリ利用者のエラー)
const std::string CommandLineParser::Impl::ERR_MSG_SETTING_DOUBLE_PREFIX("(Program Error) Prefix \"%s\" is Defined Double.");
//! エラーメッセージ --- コマンドライン引数の内容がない(実行ファイル名もない. ライブラリ利用者のエラー)
const std::string CommandLineParser::Impl::ERR_MSG_NOT_EXIST_COMMANDLINE_CONTENTS("(Program Error) There is no Commandline Contents.");
//! エラーメッセージ --- 空白の接頭辞がある(ライブラリ利用者のエラー)
const std::string CommandLineParser::Impl::ERR_MSG_EXIST_NOTHING_PREFIX_IN_SETTING("(Program Error) There is Prefix \"\" in Commandline Setting.");
//! エラーメッセージ --- 接頭辞が重複して入力されている
const std::string CommandLineParser::Impl::ERR_MSG_DOUBLE_PREFIX_INPUT("Prefix \"%s\" is Input Double.");
//! エラーメッセージ --- パラメータが不足している
const std::string CommandLineParser::Impl::ERR_MSG_PARAMETER_INSUFFICIENT("Parameter of Prefix \"%s\" is Insufficient.");
//! エラーメッセージ --- 無効なパラメータが入力されている
const std::string CommandLineParser::Impl::ERR_MSG_PARAMETER_INVALID("Parameter \"%s\" is Invalid.");

/** コンストラクタ */
CommandLineParser::CommandLineParser()
  :m_impl(std::make_unique<Impl>())
{
}

/** デストラクタ */
CommandLineParser::~CommandLineParser() {}

/**
 * エラーメッセージを作成する
 * @param[in] messageTemp エラーメッセージ
 * @param[in] keyWord     メッセージに埋め込むキーワード
 * @return エラーメッセージ
 * @remarks messageTempの「%s」の箇所にkeyWordを埋め込んで返す
 */
const std::string CommandLineParser::Impl::MakeErrorMsg(const std::string& messageTemp, const std::string& keyWord) const
{
  // バッファを用意
  char buf[Impl::BUFFER_SIZE];
  // エラーメッセージを作成して返す
  sprintf_s(buf, sizeof(buf), messageTemp.c_str(), keyWord.c_str());
  return std::string(buf);
}

/**
 * 設定された接頭辞のエラーチェックをする(エラーがある場合は例外を投げる)
 * @param[in] settings コマンドライン読み込み設定コンテナ
 * @remarks ・空白の接頭辞があればエラー <br>
 *          ・重複があればエラー
 */
void CommandLineParser::Impl::CheckSettingPrefix(const std::vector<CommandLineSetting>& settings) const
{
  //----- 設定数ループ -----//
  for (size_t i = 0; i < settings.size(); ++i)
  {
    // 空白の接頭辞があればエラーメッセージを投げる
    if (settings[i].Prefix() == "") throw ERR_MSG_EXIST_NOTHING_PREFIX_IN_SETTING;
    // 接頭文字の重複がないか確認
    for (size_t j = i + 1; j < settings.size(); ++j)
    {
      // 重複があればエラーメッセージを投げる
      if (settings[i].Prefix() == settings[j].Prefix())
      {
        throw MakeErrorMsg(ERR_MSG_SETTING_DOUBLE_PREFIX, settings[i].Prefix().c_str());
      }
    }
  }
}

/**
 * コマンド内容を記録しておく
 * @param[in] argc argvの配列サイズ
 * @param[in] argv コマンドライン引数
 */
void CommandLineParser::Impl::SaveArguments(int argc, const char* const argv[])
{
  // 実行ファイル名もない場合はエラーメッセージを投げる
  if (argc <= 0) throw ERR_MSG_NOT_EXIST_COMMANDLINE_CONTENTS;
  // 実行ファイル名を記録
  m_arguments += argv[0];
  // 引数数ループ
  for (int i = 1; i < argc; ++i)
  {
    // コマンドの内容を記録
    m_arguments += " ";
    m_arguments += argv[i];
  }
}

/**
 * 入力されたコマンドに接頭辞の重複がないかチェックする(重複がある場合は例外を投げる)
 * @param[in] settings コマンドライン読み込み設定コンテナ
 * @param[in] argc argvの配列サイズ
 * @param[in] argv コマンドライン引数
 * @return 最も後ろの接頭文字の影響範囲(接頭文字のインデックス + 後ろに取る引数の数)
 */
int CommandLineParser::Impl::CheckDoublePrefixInput(
  const std::vector<CommandLineSetting>& settings,
  int argc,
  const char* const argv[]) const
{
  // 最も後ろの接頭文字の影響範囲(接頭文字のインデックス + 後ろに取る引数の数)を記録する
  int indexLastPrefixArea = 0;
  // 読み込んだ回数を接頭辞ごとに記録するカウンタを用意
  std::unique_ptr<int[]> inputCounts(new int[settings.size()]);
  // 初期化
  for (size_t j = 0; j < settings.size(); ++j) inputCounts[j] = 0;

  // 引数数ループ
  for (int i = 1; i < argc; ++i)
  {
    // 接頭文字かどうかをチェック
    for (size_t j = 0; j < settings.size(); ++j)
    {
      // 接頭文字の場合
      if (argv[i] == settings[j].Prefix())
      {
        // 既に受け付けた接頭文字の場合はエラー
        if (inputCounts[j] > 0)
        {
          // エラーメッセージを投げる
          throw MakeErrorMsg(ERR_MSG_DOUBLE_PREFIX_INPUT, settings[j].Prefix().c_str());
        }
        // 「接頭文字のインデックス + 後ろに取る引数の数」を記録
        indexLastPrefixArea = i + settings[j].NumberOfParameter();
        // 受け付けた回数をカウントしてループを抜ける
        ++inputCounts[j];
        break;
      }
    }
  }
  // 最も後ろの接頭文字の影響範囲を返す
  return indexLastPrefixArea;
}

/**
 * コマンドオブジェクトの連想配列を作成する
 * @param[in] settings            コマンドライン読み込み設定コンテナ
 * @param[in] argc                argvの配列サイズ
 * @param[in] argv                コマンドライン引数
 * @param[in] indexLastPrefixArea 最も後ろの接頭文字の影響範囲
 */
const std::map<std::string, Command> CommandLineParser::Impl::MakeCommandArray(
  const std::vector<CommandLineSetting>& settings,
  int argc,
  const char* const argv[],
  int indexLastPrefixArea) const
{
  // 返す連想配列を用意
  std::map<std::string, Command> commands;

  //----- 接頭文字なしの引数を取得(最も後ろの接頭文字の影響範囲の直後から取得) -----//
  // 引数格納用コンテナを用意
  std::vector<std::string> arguments;
  for (int i = indexLastPrefixArea + 1; i < argc; ++i)
  {
    arguments.emplace_back(argv[i]);
  }
  // 接頭文字なしの引数として記録
  commands.emplace(
    std::piecewise_construct,
    std::forward_as_tuple(""),
    std::forward_as_tuple("", arguments));

  //----- 引数数ループ(接頭文字なしの引数以外) -----//
  for (int i = 1; i <= indexLastPrefixArea; ++i)
  {
    // 入力された接頭文字取得
    std::string prefix = argv[i];
    // 設定の中から該当する接頭文字を検索
    for (auto setting = settings.begin(); setting != settings.end(); ++setting)
    {
      // 接頭文字を比較して一致しなけばスキップ
      if (prefix != setting->Prefix()) continue;
      // 一致したら後続引数の数を取得
      int numArguments = setting->NumberOfParameter();
      // 後続引数格納コンテナを初期化
      arguments.clear();
      // 後続引数数ループ
      for (int j = 0; j < numArguments; ++j)
      {
        // 取得できる後続引数がなければエラーメッセージを投げる
        if (i >= indexLastPrefixArea)
        {
          throw MakeErrorMsg(ERR_MSG_PARAMETER_INSUFFICIENT, prefix.c_str());
        }
        // 引数カウンタを進める
        ++i;
        // 引数を格納
        arguments.emplace_back(argv[i]);
      }
      // ここまで来たらループを抜ける
      goto SUCCESS_SEARCH;
    }
    // 検索失敗時はエラーメッセージを投げる
    throw MakeErrorMsg(ERR_MSG_PARAMETER_INVALID, prefix.c_str());

    //----- 検索成功時はここにくる -----//
    SUCCESS_SEARCH:
    // 引数情報を記録
    commands.emplace(
      std::piecewise_construct,
      std::forward_as_tuple(prefix),
      std::forward_as_tuple(prefix, arguments));
  }

  // 作成した連想配列を返す
  return commands;
}

/**
 * コマンドライン引数をパースする
 * @param[in] settings コマンドライン読み込み設定コンテナ
 * @param[in] argc     argvの配列サイズ
 * @param[in] argv     コマンドライン引数
 * @return コマンドオブジェクトの連想配列
 * @attention これまでのエラーメッセージやワーニングメッセージが初期化される
 */
const std::map<std::string, Command> CommandLineParser::Parse(
  const std::vector<CommandLineSetting>& settings,
  int argc,
  const char* const argv[])
{
  // 前回読み込んだ情報を初期化する
  m_impl = std::make_unique<Impl>();

  try
  {
    // コマンド内容を記録しておく
    m_impl->SaveArguments(argc, argv);
    // 設定された接頭辞のチェック
    m_impl->CheckSettingPrefix(settings);
    // 引数の接頭辞の重複をチェックし, 最も後ろの接頭文字の影響範囲を取得
    int indexLastPrefixArea = m_impl->CheckDoublePrefixInput(settings, argc, argv);
    // コマンドオブジェクトの連想配列を作成して返す
    return m_impl->MakeCommandArray(settings, argc, argv, indexLastPrefixArea);
  }
  catch (const std::string& err)
  {
    // エラーコードを格納して空の連想配列を返す
    m_impl->m_errorMsg.emplace_back(err);
    return std::map<std::string, Command>();
  }

  // 空の連想配列を返す
  return std::map<std::string, Command>();
}

/**
 * 前回Parseで読んだコマンド内容を文字列でget
 * @return コマンド内容(exeを含む)
 */
const std::string& CommandLineParser::GetCommandArguments() const
{
  return m_impl->m_arguments;
}

/**
 * エラーメッセージを取得する
 * @return エラーメッセージのコンテナ(正常であればサイズ0)
 */
const std::vector<std::string>& CommandLineParser::ErrorMessage() const
{
  return m_impl->m_errorMsg;
}
}
