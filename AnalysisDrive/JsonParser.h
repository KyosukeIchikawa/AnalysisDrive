/**
 * @file    JsonParser.h
 * @brief   JSONパーサー
 * @author  kyo
 * @date    2015/05/07
 */
#ifndef ANALYSISDRIVE_JSONPARSER_H_
#define ANALYSISDRIVE_JSONPARSER_H_

#include <memory>
#include <string>
#include <vector>

/** Analysis Drive */
namespace adlib
{
// 前方宣言
class Json;

/**
 * JSONパーサー
 */
class JsonParser
{
private:
  //> Pimplイディオム
  class Impl;
  std::unique_ptr<Impl> m_impl;

private:
  /* コピー禁止 */
  JsonParser(const JsonParser&);
  /* 代入禁止 */
  void operator=(const JsonParser&);

public:
  /** コンストラクタ */
  JsonParser();
  /** 仮想デストラクタ */
  virtual ~JsonParser();

  /**
   * JSON形式の文字列をパースする
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
   * objectのnameが重複していればtrueを返す
   * @return 前回のParseでobjectのnameが重複していればtrueを返す
   */
  bool IsNameRepeated() const;
};

/**
 * @class   JsonParser
 * @section JsonParser_exsample exsample
 * @code
 * #include "AnalysisDrive.h"
 * #include <fstream>
 * #include <iostream>
 *
 * // 次のデータをファイル名"test.txt"で用意
 * // ※ 文字コードはUTF8(BOM有無は問わない)
 * //
 * // {
 * //   "comment": "test data",
 * //   "method": {"analysis": "FEM", "timestep": 100000, "stepsize": 0.0001},
 * //   "nonlinear": false,
 * //   "node":
 * //   [
 * //     {"coord": [0.0, 0.0, 0.0], "comment": "Origin"},
 * //     {"coord": [2.0, 0.0, 0.0]},
 * //     {"coord": [0.0, 1.2, 0.0]},
 * //     {"coord": [2.0, 1.2, 0.0]}
 * //   ]
 * // }
 * //
 * const std::string file("test.txt");
 *
 * int main()
 * {
 *   // ファイルを読み込む
 *   std::ifstream ifs;
 *   ifs.open(file);
 *
 *   // バッファにJSONのデータを格納
 *   std::string buf("");
 *   std::string tmp("");
 *   while (std::getline(ifs, tmp)) buf += tmp;
 *
 *   // パーサーを用意
 *   adlib::JsonParser parser;
 *
 *   // パースしてJSONオブジェクトを取得
 *   auto json = parser.Parse(buf);
 *   // エラー確認
 *   if (parser.ErrorMessage().size() > 0)
 *   {
 *     std::cout << "ERROR" << std::endl;
 *     return -1;
 *   }
 *
 *   // jsonがobject型でなければエラー
 *   if (!(json.IsObject())) return -1;
 *   // objectを取得
 *   auto jsonObj = json.GetObject();
 *
 *   // "method"があり, objectであることを確認
 *   if (jsonObj["method"].IsObject())
 *   {
 *     // methodの値をobjectとして取得
 *     auto method = jsonObj["method"].GetObject();
 *
 *     // "analysis"があり, stringであれば出力
 *     if (method["analysis"].IsString()) std::cout << "analysis " << method["analysis"].GetString() << std::endl;
 *
 *     // "timestep"があり, intもしくはdoubleであればint型として出力
 *     if (method["timestep"].IsNumber()) std::cout << "timestep " << method["timestep"].GetNumberInt() << std::endl;
 *
 *     // "stepsize"があり, intもしくはdoubleであればdouble型として出力
 *     if (method["stepsize"].IsNumber()) std::cout << "stepsize " << method["stepsize"].GetNumberDouble() << std::endl; *
 *
 *   }
 *
 *   // "node"があり, arrayであることを確認
 *   if (jsonObj["node"].IsArray())
 *   {
 *     // nodeの値をarrayとして取得
 *     auto nodes = jsonObj["node"].GetArray();
 *     // arrayの成分数だけループする
 *     // ※ nodes.size()でサイズを取得して, インデックスでループをまわしても良い
 *     for (auto node = nodes.begin(); node != nodes.end(); ++node)
 *     {
 *       // objectであることを確認
 *       if (node->IsObject())
 *       {
 *         auto nodeObj = node->GetObject();
 *         // "coord"があり, arrayであることを確認
 *         if (nodeObj["coord"].IsArray())
 *         {
 *           // coordの値をarrayとして取得
 *           auto coords = nodeObj["coord"].GetArray();
 *           // 全要素出力
 *           std::cout << "coord " << std::flush;
 *           for (auto coord = coords.begin(); coord != coords.end(); ++coord)
 *           {
 *             std::cout << coord->GetNumberDouble() << std::flush;
 *           }
 *           std::cout << std::endl;
 *         }
 *       }
 *       else
 *       {
 *         continue;
 *       }
 *     }
 *   }
 *
 *   // "linear"はないことを確認
 *   if (jsonObj["linear"].IsNull())
 *   {
 *     std::cout << "linear ????" << std::endl;
 *   }
 *
 *   return 0;
 * }

 * @endcode
 */
}
#endif // ANALYSISDRIVE_JSONPARSER_H_
