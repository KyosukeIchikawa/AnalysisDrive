/**
 * @file   JsonParser.cpp
 * @brief  JSONパーサー
 * @author kyo
 * @date   2015/05/07
 */
#include "JsonParser.h"
#include "Json.h"
#include <memory>
#include <vector>
#include <unordered_map>
#include <string>

/** Analysis Drive */
namespace adlib
{
//> Pimplイディオム
class JsonParser::Impl
{
public:
  //> リターンコード
  enum RETURN_CODE
  {
    SUCCESS, /*!< 成功 */
    FAILURE  /*!< 失敗 */
  };

  //> UNICODEエスケープシーケンスの4BYTEの意
  static const size_t ESCAPE_SEQUENCE_4BYTE;
  //> 16進数の意
  static const int HEXADECIMAL_16;
  //> UTF8のBOMサイズ
  static const int BOM_UTF8_SIZE;
  //> UTF8のBOM
  static const char BOM_UTF8[];
  //> エラーメッセージを格納するバッファサイズ
  static const size_t BUFFER_SIZE;
  //> エラーメッセージ --- JSONのパース失敗
  static const std::string ERR_MSG_PARSE_FAILURE;
  //> エラーメッセージ --- 0接頭のnumber型がある
  static const std::string ERR_MSG_0HEAD_NUMBER;
  //> エラーメッセージ --- number型に無効な文字が含まれている
  static const std::string ERR_MSG_BAD_WORD_IN_NUMBER;
  //> エラーメッセージ --- number型(int型)が範囲外の値
  static const std::string ERR_MSG_INT_NUMBER_OUT_OF_RANGE;
  //> エラーメッセージ --- number型(double型)が範囲外の値
  static const std::string ERR_MSG_DOUBLE_NUMBER_OUT_OF_RANGE;
  //> エラーメッセージ --- '.'の後続に数値が無い
  static const std::string ERR_MSG_NOT_DIGIT_FOLLOWER_DOT;
  //> エラーメッセージ --- 'e', 'E'の後続に数値が無い
  static const std::string ERR_MSG_NOT_DIGIT_FOLLOWER_EXP;
  //> エラーメッセージ --- 文字列を閉じる'"'がない
  static const std::string ERR_MSG_NOT_EXIST_DOUBLE_QUOTATION;
  //> エラーメッセージ --- 不正なエスケープ文字がある
  static const std::string ERR_MSG_ESCAPE_CHARACTER_FAILURE;
  //> エラーメッセージ --- サロゲート領域のUNICODEがある
  static const std::string ERR_MSG_EXIST_CODE_IN_SURROGATE;
  //> エラーメッセージ --- objectを閉じる'}'がない
  static const std::string ERR_MSG_NOT_EXIST_BRACKET_IN_OBJ;
  //> エラーメッセージ --- object内の2つ目以降の値の前に','がない
  static const std::string ERR_MSG_NOT_EXIST_COMMA_IN_OBJ;
  //> エラーメッセージ --- objectに文字列開始の'"'がない
  static const std::string ERR_MSG_NOT_EXIST_DOUBLE_QUOTATION_IN_OBJ;
  //> エラーメッセージ --- objectに':'がない
  static const std::string ERR_MSG_NOT_EXIST_COLON_IN_OBJ;
  //> エラーメッセージ --- arrayを閉じる']'がない
  static const std::string ERR_MSG_NOT_EXIST_BRACKET_IN_ARRAY;
  //> エラーメッセージ --- array内の2つ目以降の値の前に','がない
  static const std::string ERR_MSG_NOT_EXIST_COMMA_IN_ARRAY;
  //> エラーメッセージ --- 不正な値がある
  static const std::string ERR_MSG_VALUE_FAILURE;
  //> ワーニングメッセージ --- objectのnameが重複している
  static const std::string WRN_MSG_NAME_REPEATED;

  //> エラーメッセージコンテナ
  std::vector<std::string> m_errorMsg;
  //> ワーニングメッセージコンテナ
  std::vector<std::string> m_warningMsg;
  //> objectのname重複フラグ
  bool m_nameRepeated;
  //> パース対象の文字列
  const std::string& m_target;
  //> パース対象の文字列のカーソル(次に読み込もうとする位置)
  size_t m_i;

  /**
   * コンストラクタ
   * @param[in] target パース対象の文字列
   */
  Impl(const std::string& target)
    :m_nameRepeated(false),
     m_target(target),
     m_i(0)
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
   * 次の1文字を取得(ホワイトスペースはスキップ)
   * @param[out] word 格納先
   * @return リターンコード(取得成功時はSUCCESS)
   * @remarks 本関数終了後, カーソルは次の1文字を指す(ホワイトスペースはスキップ)
   */
  Impl::RETURN_CODE SkipWhitespaceAndGetWord(char* word);

  /**
   * UTF8のBOMがあればスキップする
   */
  void SkipBOM();

  /**
   * エスケープシーケンスを文字列に変換する
   * @param[in] hex UNICODE16進数
   */
  const std::string ConversionEscapeSequence(int hex);

  /**
   * string型をパースする
   * @remarks 本関数終了後, カーソルはstring型の次の1文字を指す
   */
  Json ParseString();

  /**
   * number型をパースする
   * @remarks 本関数終了後, カーソルはnumber型の次の1文字を指す
   */
  Json ParseNumber();

  /**
   * object型をパースする
   * @remarks 本関数終了後, カーソルはobject型の次の1文字を指す
   */
  Json ParseObject();

  /**
   * array型をパースする
   * @remarks 本関数終了後, カーソルはarray型の次の1文字を指す
   */
  Json ParseArray();

  /**
   * 値をチェックする(値が不正な場合は例外を投げる)
   * @param[in] checkString チェックする値
   * @remarks 本関数終了後, カーソルは値の次の1文字を指す
   */
  void CheckValue(const std::string& checkString);

  /**
   * valueをパースする
   */
  Json ParseValue();
};

//> UNICODEエスケープシーケンスの4BYTEの意
const size_t JsonParser::Impl::ESCAPE_SEQUENCE_4BYTE = 4;
//> 16進数の意
const int JsonParser::Impl::HEXADECIMAL_16 = 16;
//> UTF8のBOMサイズ
const int JsonParser::Impl::BOM_UTF8_SIZE = 3;
//> UTF8のBOM
const char JsonParser::Impl::BOM_UTF8[] = { char(0xEF), char(0xBB), char(0xBF) };
//> エラーメッセージを格納するバッファサイズ
const size_t JsonParser::Impl::BUFFER_SIZE = 256;
//> エラーメッセージ --- JSONのパース失敗
const std::string JsonParser::Impl::ERR_MSG_PARSE_FAILURE("Parse Failure.");
//> エラーメッセージ --- 0接頭のnumber型がある
const std::string JsonParser::Impl::ERR_MSG_0HEAD_NUMBER("There is '0' Prefixial Number.");
//> エラーメッセージ --- number型に無効な文字が含まれている
const std::string JsonParser::Impl::ERR_MSG_BAD_WORD_IN_NUMBER("There is Bad Word in Number.");
//> エラーメッセージ --- number型(int型)が範囲外の値
const std::string JsonParser::Impl::ERR_MSG_INT_NUMBER_OUT_OF_RANGE("\"%s\" is out of Integer Range.");
//> エラーメッセージ --- number型(double型)が範囲外の値
const std::string JsonParser::Impl::ERR_MSG_DOUBLE_NUMBER_OUT_OF_RANGE("\"%s\" is out of Double Range.");
//> エラーメッセージ --- '.'の後続に数値が無い
const std::string JsonParser::Impl::ERR_MSG_NOT_DIGIT_FOLLOWER_DOT("There is not '0'~'9' Follower of '.' in Number.");
//> エラーメッセージ --- 'e', 'E'の後続に数値が無い
const std::string JsonParser::Impl::ERR_MSG_NOT_DIGIT_FOLLOWER_EXP("There is not '0'~'9' Follower of 'e' or 'E' in Number.");
//> エラーメッセージ --- 文字列を閉じる'"'がない
const std::string JsonParser::Impl::ERR_MSG_NOT_EXIST_DOUBLE_QUOTATION("There is not '\"' end of \"%s\".");
//> エラーメッセージ --- 不正なエスケープ文字がある
const std::string JsonParser::Impl::ERR_MSG_ESCAPE_CHARACTER_FAILURE("Escape Character of \"%s\" is Failure.");
//> エラーメッセージ --- サロゲート領域のUNICODEがある
const std::string JsonParser::Impl::ERR_MSG_EXIST_CODE_IN_SURROGATE("There is code in the range U+D800 ~ U+DFFF (surrogate).");
//> エラーメッセージ --- objectを閉じる'}'がない
const std::string JsonParser::Impl::ERR_MSG_NOT_EXIST_BRACKET_IN_OBJ("There is not '}' in object.");
//> エラーメッセージ --- object内の2つ目以降の値の前に','がない
const std::string JsonParser::Impl::ERR_MSG_NOT_EXIST_COMMA_IN_OBJ("There is not ',' in object.");
//> エラーメッセージ --- objectに文字列開始の'"'がない
const std::string JsonParser::Impl::ERR_MSG_NOT_EXIST_DOUBLE_QUOTATION_IN_OBJ("There is not '\"' in object.");
//> エラーメッセージ --- objectに':'がない
const std::string JsonParser::Impl::ERR_MSG_NOT_EXIST_COLON_IN_OBJ("There is not ':' in object.");
//> エラーメッセージ --- arrayを閉じる']'がない
const std::string JsonParser::Impl::ERR_MSG_NOT_EXIST_BRACKET_IN_ARRAY("There is not ']' in array.");
//> エラーメッセージ --- array内の2つ目以降の値の前に','がない
const std::string JsonParser::Impl::ERR_MSG_NOT_EXIST_COMMA_IN_ARRAY("There is not ',' in array.");
//> エラーメッセージ --- 不正な値がある
const std::string JsonParser::Impl::ERR_MSG_VALUE_FAILURE("Value \"%s\" is Failure.");
//> ワーニングメッセージ --- objectのnameが重複している
const std::string JsonParser::Impl::WRN_MSG_NAME_REPEATED("Name \"%s\" Repeated, and Later Name is Given Priority.");

/**
 * エラーメッセージを作成する
 * @param[in] messageTemp エラーメッセージ
 * @param[in] keyWord     メッセージに埋め込むキーワード
 * @return エラーメッセージ
 * @remarks messageTempの「%s」の箇所にkeyWordを埋め込んで返す
 */
const std::string JsonParser::Impl::MakeErrorMsg(const std::string& messageTemp, const std::string& keyWord) const
{
  // バッファを用意
  char buf[Impl::BUFFER_SIZE];
  // エラーメッセージを作成して返す
  sprintf_s(buf, sizeof(buf), messageTemp.c_str(), keyWord.c_str());
  return std::string(buf);
}

/**
 * 次の1文字を取得(ホワイトスペースはスキップ)
 * @param[out] word 格納先
 * @return リターンコード(取得成功時はSUCCESS)
 * @remarks 本関数終了後, カーソルは次の1文字を指す(ホワイトスペースはスキップ)
 */
JsonParser::Impl::RETURN_CODE JsonParser::Impl::SkipWhitespaceAndGetWord(char* word)
{
  // 文字が無ければ取得失敗
  if (m_i >= m_target.size())
  {
    return FAILURE;
  }
  while (m_target[m_i] == ' ' // 空白
    || m_target[m_i] == '\r'  // 復帰
    || m_target[m_i] == '\n'  // 改行
    || m_target[m_i] == '\t') // タブ
  {
    ++m_i;
    // 文字が無ければ取得失敗
    if (m_i >= m_target.size())
    {
      return FAILURE;
    }
  }
  // 文字を格納
  *word = m_target[m_i];
  ++m_i;
  // 取得成功
  return SUCCESS;
}

/**
 * UTF8のBOMがあればスキップする
 */
void JsonParser::Impl::SkipBOM()
{
  // BOMを取得
  std::string bom(m_target.substr(m_i, BOM_UTF8_SIZE));
  // UTF8のBOMサイズ分取得できなかった場合はBOMなしと判断して戻る
  if (bom.size() != BOM_UTF8_SIZE) return;
  // BOMチェック
  for (int i = 0; i < BOM_UTF8_SIZE; ++i)
  {
    // int型に変換してチェック(UTF8のBOMでなければ戻る)
    if (BOM_UTF8[i] != bom[i]) return;
  }
  // ここまで来たらbomなのでスキップ
  m_i += BOM_UTF8_SIZE;
}

/**
 * エスケープシーケンスを文字列に変換する
 * @param[in] hex UNICODE16進数
 */
const std::string JsonParser::Impl::ConversionEscapeSequence(int hex)
{
  // 返す文字列
  std::string returnString("");

  // ASCII 0x0000 ~ 0x007F (0000|0000|0xxx|xxxx)
  if (hex <= 0x007F)
  {
    // 第1バイト 0xxx|xxxx
    returnString += static_cast<char>(hex);
  }
  // 0x0080 ~ 0x07FF (0000|0xxx|xxyy|yyyy)
  else if (hex <= 0x07FF)
  {
    // 第1バイト 110x|xxxx
    // (hex >> 6)で 0000|0000|000x|xxxx
    // (| 0x00C0)で 0000|0000|1100|0000 の1の箇所を付加
    returnString += static_cast<char>((hex >> 6) | 0x00C0);
    // 第2バイト 10yy|yyyy
    // (& 0x003F)で 0000|0000|0011|1111 の0の箇所を除外
    // (| 0x0080)で 0000|0000|1000|0000 の1の箇所を付加
    returnString += static_cast<char>((hex & 0x003F) | 0x0080);
  }
  // 0x0800 ~ 0xD7FF (xxxx|yyyy|yyzz|zzzz)
  else if (hex <= 0xD7FF)
  {
    // 第1バイト 1110|xxxx
    // (hex >>12)で 0000|0000|0000|xxxx
    // (| 0x00E0)で 0000|0000|1110|0000 の1の箇所を付加
    returnString += static_cast<char>((hex >> 12) | 0x00E0);
    // 第2バイト 10yy|yyyy
    // (hex >> 6)で 0000|00xx|xxyy|yyyy
    // (& 0x003F)で 0000|0000|0011|1111 の0の箇所を除外
    // (| 0x0080)で 0000|0000|1000|0000 の1の箇所を付加
    returnString += static_cast<char>(((hex >> 6) & 0x003F) | 0x0080);
    // 第3バイト 10zz|zzzz
    // (& 0x003F)で 0000|0000|0011|1111 の0の箇所を除外
    // (| 0x0080)で 0000|0000|1000|0000 の1の箇所を付加
    returnString += static_cast<char>((hex & 0x003F) | 0x0080);
  }
  // サロゲート領域 0x0800 ~ 0xDFFF (使用禁止)
  else if (hex <= 0xDFFF)
  {
    // エラーメッセージを投げる
    throw Impl::ERR_MSG_EXIST_CODE_IN_SURROGATE;
  }
  // 0xE000 ~ 0xFFFF (xxxx|yyyy|yyzz|zzzz)
  else if (hex <= 0xFFFF)
  {
    // 第1バイト 1110|xxxx
    // (hex >>12)で 0000|0000|0000|xxxx
    // (| 0x00E0)で 0000|0000|1110|0000 の1の箇所を付加
    returnString += static_cast<char>((hex >> 12) | 0x00E0);
    // 第2バイト 10yy|yyyy
    // (hex >> 6)で 0000|00xx|xxyy|yyyy
    // (& 0x003F)で 0000|0000|0011|1111 の0の箇所を除外
    // (| 0x0080)で 0000|0000|1000|0000 の1の箇所を付加
    returnString += static_cast<char>(((hex >> 6) & 0x003F) | 0x0080);
    // 第3バイト 10zz|zzzz
    // (& 0x003F)で 0000|0000|0011|1111 の0の箇所を除外
    // (| 0x0080)で 0000|0000|1000|0000 の1の箇所を付加
    returnString += static_cast<char>((hex & 0x003F) | 0x0080);
  }
  // 0x010000 ~ 0x10FFFF (x|xxyy|yyyy|zzzz|zzww|wwww)
  else
  {
    // 第1バイト 1111|0xxx
    // (hex >>18)で 0000|0000|0000|0xxx
    // (| 0x00F0)で 0000|0000|1111|0000 の1の箇所を付加
    returnString += static_cast<char>((hex >> 18) | 0x00F0);
    // 第2バイト 10yy|yyyy
    // (hex >>12)で 0000|000x|xxyy|yyyy
    // (& 0x003F)で 0000|0000|0011|1111 の0の箇所を除外
    // (| 0x0080)で 0000|0000|1000|0000 の1の箇所を付加
    returnString += static_cast<char>(((hex >> 12) & 0x003F) | 0x0080);
    // 第3バイト 10zz|zzzz
    // (hex >> 6)で 0xxx|yyyy|yyzz|zzzz
    // (& 0x003F)で 0000|0000|0011|1111 の0の箇所を除外
    // (| 0x0080)で 0000|0000|1000|0000 の1の箇所を付加
    returnString += static_cast<char>(((hex >> 6) & 0x003F) | 0x0080);
    // 第4バイト 10ww|wwww
    // (& 0x003F)で 0000|0000|0011|1111 の0の箇所を除外
    // (| 0x0080)で 0000|0000|1000|0000 の1の箇所を付加
    returnString += static_cast<char>((hex & 0x003F) | 0x0080);
  }

  return returnString;
}

/**
 * string型をパースする
 * @remarks 本関数終了後, カーソルはstring型の次の1文字を指す
 */
Json JsonParser::Impl::ParseString()
{
  // <string>
  //          
  // *--->'"'------------------------------------->'"'--->*
  //          |                                 ^
  //          |                                 |
  //          |<--------------------------------|
  //          |                                 |
  //          |                                 | 
  //          |------> UNICODE character ------>|
  //          |        (except '"' or '\')      |
  //          |                                 |
  //          V                                 | 
  //          --->'\'----->'"'----------------->| quotation mark
  //                   |                        |
  //                   |-->'\'----------------->| reverse solidus
  //                   |-->'/'----------------->| solidus
  //                   |-->'b'----------------->| backspace
  //                   |-->'f'----------------->| formfeed
  //                   |-->'n'----------------->| newline
  //                   |-->'r'----------------->| carriage return
  //                   |-->'t'----------------->| horizontal tab
  //                   V-->'u'-->4 hex digits-->| escape sequence
  //

  // バッファを用意
  std::string str("");

  // 最初の'"'をスキップ
  ++m_i;

  // '"'が来るまでループ
  while (true)
  {
    // 文字列の最後を超えたらエラー
    if (m_i >= m_target.size())
    {
      // エラーメッセージを投げる
      throw MakeErrorMsg(Impl::ERR_MSG_NOT_EXIST_DOUBLE_QUOTATION, '\"' + str);
    }
    // 1文字取得
    char word = m_target[m_i];
    ++m_i;

    // '"'が来たら終了
    if (word == '\"')
    {
      // ループを抜ける
      break;
    }

    //----- エスケープ文字でない場合 -----//
    if (word != '\\')
    {
      // 格納して次へ
      str += word;
      continue;
    }

    //----- エスケープ文字の場合 -----//
    // 文字列の最後を超えていたらエラー
    if (m_i >= m_target.size())
    {
      // エラーメッセージを投げる
      str += word;
      throw MakeErrorMsg(Impl::ERR_MSG_ESCAPE_CHARACTER_FAILURE, '\"' + str);
    }

    // 1文字取得
    word = m_target[m_i];
    ++m_i;

    // エスケープ文字分岐
    switch (word)
    {
      case '\"': // quotation mark
      case '\\': // reverse solidus
      case '/':  // solidus
        str += word;
        break;
      case 'b': // backspace
        str += '\b';
        break;
      case 'f': // formfeed
        str += '\f';
        break;
      case 'n': // newline
        str += '\n';
        break;
      case 'r': // carriage return
        str += '\r';
        break;
      case 't': // horizontal tab
        str += '\t';
        break;
      case 'u': // escape sequence
      {
        // エスケープシーケンスの値4BYTEを取得
        std::string escapeSequence = m_target.substr(m_i, ESCAPE_SEQUENCE_4BYTE);
        m_i += escapeSequence.size();
        // 4BYTE取得できなければエラー
        if (escapeSequence.size() < ESCAPE_SEQUENCE_4BYTE)
        {
          // エラーメッセージを投げる
          str += word;
          throw MakeErrorMsg(Impl::ERR_MSG_ESCAPE_CHARACTER_FAILURE, '\"' + str);
        }
        // 16進数でなければエラー
        for (int i = 0; i < ESCAPE_SEQUENCE_4BYTE; ++i)
        {
          if (escapeSequence[i] >= '0' && escapeSequence[i] <= '9') continue;
          if (escapeSequence[i] >= 'a' && escapeSequence[i] <= 'f') continue;
          if (escapeSequence[i] >= 'A' && escapeSequence[i] <= 'F') continue;
          // エラーメッセージを投げる
          str += word;
          throw MakeErrorMsg(Impl::ERR_MSG_ESCAPE_CHARACTER_FAILURE, '\"' + str);
        }
        // int型(10進数)に変換する
        int hex = std::stoi(escapeSequence, nullptr, Impl::HEXADECIMAL_16);
        // エスケープシーケンスを変換してstrに加算する
        str += ConversionEscapeSequence(hex);
      }
      break;
      default:
        // エラーメッセージを投げる
        str += word;
        throw MakeErrorMsg(Impl::ERR_MSG_ESCAPE_CHARACTER_FAILURE, '\"' + str);
    }
  }

  //***** string型Jsonオブジェクトを返す *****//
  return Json(str);
}

/**
 * number型をパースする
 * @remarks 本関数終了後, カーソルはnumber型の次の1文字を指す
 */
Json JsonParser::Impl::ParseNumber()
{
  // <number>                               <------------
  //                                        |           ^
  //                               (double) V           |                       
  //                               ---'.'----->'0'~'9'-->
  //                               ^                    |
  //                               |                    V
  // *----------------->'0'---------------------------------------------------------------------->*
  //    |       ^  |           ^                          |                                     ^
  //    V       |  V           |                          V (double)                            |
  //    -->'-'-->  -->'1'~'9'-->                          --> 'e', 'E' --> '+', '-'---->'0'~'9'->
  //               ^           |                                        |          ^ ^          |
  //               |           V                                        V          | |          V
  //               <--'0'~'9'<--                                        -----------> <-----------
  //

  // 開始インデックスをメモ
  size_t start = m_i;

  // '-'確認
  if (m_target[m_i] == '-') ++m_i;

  // '0'確認
  if (m_target[m_i] == '0')
  {
    ++m_i;
    // '0'から始まるnumber型で次も'0'~'9'であれば無効
    if ('0' <= m_target[m_i] && m_target[m_i] <= '9')
    {
      // エラーメッセージを投げる
      throw Impl::ERR_MSG_0HEAD_NUMBER;
    }
  }
  // '1'~'9'確認
  else if ('1' <= m_target[m_i] && m_target[m_i] <= '9')
  {
    ++m_i;
    // 後続の'0'~'9'を確認
    while ('0' <= m_target[m_i] && m_target[m_i] <= '9') ++m_i;
  }
  // その他はエラー
  else
  {
    // エラーメッセージを投げる
    throw Impl::ERR_MSG_BAD_WORD_IN_NUMBER;
  }

  // int型か確認
  if (m_target[m_i] != '.'
    && m_target[m_i] != 'e'
    && m_target[m_i] != 'E')
  {
    try
    {
      // int型に変換(C+11STLであれば, stringの内部バッファは連続領域に配置される)
      int number = std::stoi(m_target.c_str() + start);
      //***** int型Jsonオブジェクトを返す *****//
      return Json(number);
    }
    // 範囲外の値になった場合
    catch (std::out_of_range e)
    {
      // エラーメッセージを投げる
      throw MakeErrorMsg(Impl::ERR_MSG_INT_NUMBER_OUT_OF_RANGE, m_target.c_str() + start);
    }
    // 数値への変換が行われない場合
    catch (std::invalid_argument e)
    {
      // エラーメッセージを投げる
      throw Impl::ERR_MSG_BAD_WORD_IN_NUMBER;
    }
  }

  // '.'確認
  if (m_target[m_i] == '.')
  {
    ++m_i;
    // 後続に'0'~'9'が無ければエラー
    if (m_target[m_i] < '0' || '9' < m_target[m_i])
    {
      // エラーメッセージを投げる
      throw Impl::ERR_MSG_NOT_DIGIT_FOLLOWER_DOT;
    }
    // 後続の'0'~'9'を確認
    while ('0' <= m_target[m_i] && m_target[m_i] <= '9') ++m_i;
  }

  // 'e', 'E'確認
  if (m_target[m_i] == 'e' || m_target[m_i] == 'E')
  {
    ++m_i;
    // '+', '-'確認
    if (m_target[m_i] == '+' || m_target[m_i] == '-') ++m_i;
    // 後続に'0'~'9'が無ければエラー
    if (m_target[m_i] < '0' || '9' < m_target[m_i])
    {
      // エラーメッセージを投げる
      throw Impl::ERR_MSG_NOT_DIGIT_FOLLOWER_EXP;
    }
    // 後続の'0'~'9'を確認
    while ('0' <= m_target[m_i] && m_target[m_i] <= '9') ++m_i;
  }

  try
  {
    // double型に変換(C+11STLであれば, stringの内部バッファは連続領域に配置される)
    double number = std::stod(m_target.c_str() + start);
    //***** double型Jsonオブジェクトを返す *****//
    return Json(number);
  }
  // 範囲外の値になった場合
  catch (std::out_of_range e)
  {
    // エラーメッセージを投げる
    throw MakeErrorMsg(Impl::ERR_MSG_DOUBLE_NUMBER_OUT_OF_RANGE, m_target.c_str() + start);
  }
  // 数値への変換が行われない場合
  catch (std::invalid_argument e)
  {
    // エラーメッセージを投げる
    throw Impl::ERR_MSG_BAD_WORD_IN_NUMBER;
  }
}

/**
 * object型をパースする
 * @remarks 本関数終了後, カーソルはobject型の次の1文字を指す
 */
Json JsonParser::Impl::ParseObject()
{
  // <object>
  //          
  // *--->'{'------------------------------------>'}'--->*
  //          |                                ^
  //          |                                |
  //          |<--------------','<-------------|
  //          |                                |
  //          V                                | 
  //          ---> string --->':'---> value --->
  //

  // バッファを用意
  Json::Object obj;
  // 1st valueフラグを用意(最初のvalueであればtrueとなる)
  bool flag1stValue = true;

  // '{'をスキップ
  ++m_i;

  // '}'が来るまでループ
  while (true)
  {
    // 文字の格納先を用意
    char word = '\0';
    // 確認のため, 次の文字を取得
    if (this->SkipWhitespaceAndGetWord(&word) != Impl::SUCCESS)
    {
      // エラーメッセージを投げる
      throw Impl::ERR_MSG_NOT_EXIST_BRACKET_IN_OBJ;
    }

    // '}'が来たら終了
    if (word == '}')
    {
      // ループを抜ける
      break;
    }

    // 最初のvalueでないとき
    if (flag1stValue == false)
    {
      // ','でなければエラー
      if (word != ',')
      {
        // エラーメッセージを投げる
        throw Impl::ERR_MSG_NOT_EXIST_COMMA_IN_OBJ;
      }
      // 確認のため, 次の文字を取得
      if (this->SkipWhitespaceAndGetWord(&word) != Impl::SUCCESS)
      {
        // エラーメッセージを投げる
        throw Impl::ERR_MSG_NOT_EXIST_BRACKET_IN_OBJ;
      }
    }

    // 1文字戻す
    --m_i;
    // stringを格納するバッファを用意
    std::string key;
    // string型であることを確認してパース(Parse関数に対応するようにswitch文で記載)
    switch (word)
    {
      case '"': // string型
      {
        auto stringJson = this->ParseString();
        // 失敗時はnull型Jsonオブジェクトを返す
        if (stringJson.IsNull()) return Json(nullptr);
        // stringを取得
        key = stringJson.GetString();
        // キーが重複している場合
        if (obj.count(key) > 0)
        {
          // 重複フラグをたてる
          m_nameRepeated = true;
          // ワーニングメッセージ格納
          m_warningMsg.emplace_back(
            MakeErrorMsg(Impl::WRN_MSG_NAME_REPEATED, key));
        }
        break;
      }
      default: // others
        // エラーメッセージを投げる
        throw Impl::ERR_MSG_NOT_EXIST_DOUBLE_QUOTATION_IN_OBJ;
    }

    // 確認のため, 次の文字を取得(':'が取得できなければエラー)
    if (this->SkipWhitespaceAndGetWord(&word) != Impl::SUCCESS
      || word != ':')
    {
      // エラーメッセージを投げる
      throw Impl::ERR_MSG_NOT_EXIST_COLON_IN_OBJ;
    }

    // パースして格納
    obj[key] = this->ParseValue();
    // 最初のvalueを読み終わったのでフラグをおろす
    flag1stValue = false;
  }
  //***** object型Jsonオブジェクトを返す *****//
  return Json(obj);
}

/**
 * array型をパースする
 * @remarks 本関数終了後, カーソルはarray型の次の1文字を指す
 */
Json JsonParser::Impl::ParseArray()
{
  // <array>
  //          
  // *--->'['------------------------------------>']'--->*
  //          |                                ^
  //          |                                |
  //          |<--------------','<-------------|
  //          |                                |
  //          V                                | 
  //          -------------> value ------------>
  //

  // バッファを用意
  Json::Array arr;
  // 1st valueフラグを用意(最初のvalueであればtrueとなる)
  bool flag1stValue = true;

  // '['をスキップ
  ++m_i;

  // ']'が来るまでループ
  while (true)
  {
    // 文字の格納先を用意
    char word = '\0';
    // 確認のため, 次の文字を取得
    if (this->SkipWhitespaceAndGetWord(&word) != Impl::SUCCESS)
    {
      // エラーメッセージを投げる
      throw Impl::ERR_MSG_NOT_EXIST_BRACKET_IN_ARRAY;
    }

    // ']'が来たら終了
    if (word == ']')
    {
      // ループを抜ける
      break;
    }

    // 最初のvalueでないとき
    if (flag1stValue == false)
    {
      // ','でなければエラー
      if (word != ',')
      {
        // エラーメッセージを投げる
        throw Impl::ERR_MSG_NOT_EXIST_COMMA_IN_ARRAY;
      }
      // 確認のため, 次の文字を取得
      if (this->SkipWhitespaceAndGetWord(&word) != Impl::SUCCESS)
      {
        // エラーメッセージを投げる
        throw Impl::ERR_MSG_NOT_EXIST_BRACKET_IN_ARRAY;
      }
    }

    // 1文字戻す
    --m_i;
    // パースして格納
    arr.emplace_back(this->ParseValue());
    // 最初のvalueを読み終わったのでフラグをおろす
    flag1stValue = false;
  }
  //***** array型Jsonオブジェクトを返す *****//
  return Json(arr);
}

/**
 * 値をチェックする(値が不正な場合は例外を投げる)
 * @param[in] checkString チェックする値
 * @remarks 本関数終了後, カーソルは値の次の1文字を指す
 */
void JsonParser::Impl::CheckValue(const std::string& checkString)
{
  for (size_t i = 0; i < checkString.size(); ++i)
  {
    // 文字列の最後を超えているときはエラー
    if (m_i >= m_target.size())
    {
      // エラーメッセージを投げる
      throw MakeErrorMsg(JsonParser::Impl::ERR_MSG_VALUE_FAILURE, m_target.substr(m_i - 1 - i, i - 1));
    }
    // 文字列が値と一致しないときはエラー
    if (m_target[m_i] != checkString[i])
    {
      // エラーメッセージを投げる
      throw MakeErrorMsg(JsonParser::Impl::ERR_MSG_VALUE_FAILURE, m_target.substr(m_i - i, i));
    }
    // エラーでなければ次を確認する
    ++m_i;
  }
}

/**
 * valueをパースする
 */
Json JsonParser::Impl::ParseValue()
{
  // 文字の格納先を用意
  char word = '\0';

  // 確認のため, 次の文字を取得
  if (this->SkipWhitespaceAndGetWord(&word) != Impl::SUCCESS)
  {
    // 取得できなかった場合はエラーメッセージを投げる
    throw Impl::ERR_MSG_PARSE_FAILURE;
  }
  // 1文字戻す
  --(this->m_i);

  // valueは次のように分岐する
  //
  // *----> string型 ----->*
  //    |              ^
  //    V              |
  //    --> number型 -->
  //    |              ^
  //    V              |
  //    --> object型 -->
  //    |              ^
  //    V              |
  //    --> array型  -->
  //    |              ^
  //    V              |
  //    --> true     -->
  //    |              ^
  //    V              |
  //    --> false    -->
  //    |              ^
  //    V              |
  //    --> null 　  -->
  //
  switch (word)
  {
    case '"': // string型
      return this->ParseString();

    case '-': // number型
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      return this->ParseNumber();

    case '{': // object型
      return this->ParseObject();

    case '[': // array型
      return this->ParseArray();

    case 't': // true
      // trueであることをチェックしてから, bool型Jsonオブジェクト(true)を返す
      this->CheckValue("true");
      return Json(true);

    case 'f': // false
      // falseであることをチェックしてから, bool型Jsonオブジェクト(false)を返す
      this->CheckValue("false");
      return Json(false);

    case 'n': // null
      // nullであることをチェックしてから, nullのJsonオブジェクトを返す
      this->CheckValue("null");
      return Json(nullptr);

    default: // others
      // エラーメッセージを投げる
      throw JsonParser::Impl::ERR_MSG_PARSE_FAILURE;
  }
}

/** コンストラクタ */
JsonParser::JsonParser()
  : m_impl(std::make_unique<Impl>(""))
{
}

/** デストラクタ */
JsonParser::~JsonParser() {}

/**
 * JSON形式の文字列をパースする
 * @param[in] target パース対象の文字列
 * @return Jsonオブジェクト
 * @attention これまでのエラーメッセージやワーニングメッセージが初期化される
 */
Json JsonParser::Parse(const std::string& target)
{
  // Pimplを初期化
  m_impl = std::make_unique<Impl>(target);
  // UTF8のBOMがあればスキップする
  m_impl->SkipBOM();

  try
  {
    // パースする
    return m_impl->ParseValue();
  }
  catch (const std::string& err)
  {
    // エラーメッセージ格納
    m_impl->m_errorMsg.emplace_back(err);
    // 失敗時はnull型Jsonオブジェクトを返す
    return Json(nullptr);
  }
}

/**
 * エラーメッセージを取得する
 * @return エラーメッセージのコンテナ(正常であればサイズ0)
 */
const std::vector<std::string>& JsonParser::ErrorMessage() const
{
  return m_impl->m_errorMsg;
}

/**
 * ワーニングメッセージを取得する
 * @return ワーニングメッセージのコンテナ(正常であればサイズ0)
 */
const std::vector<std::string>& JsonParser::WarningMessage() const
{
  return m_impl->m_warningMsg;
}

/**
 * objectのnameが重複していればtrueを返す
 * @return 前回のParseでobjectのnameが重複していればtrueを返す
 */
bool JsonParser::IsNameRepeated() const
{
  return m_impl->m_nameRepeated;
}
}
