// 作成者: avishorp@gmail.com
//
// このライブラリはフリーソフトウェアです。
// フリーソフトウェア財団が発行したGNU劣等一般公衆利用許諾契約書
// (GNU Lesser General Public License)、バージョン2.1、または
// それ以降の任意のバージョンに従って、再配布および修正を行うことができます。
//
// このライブラリは、それが役立つことを期待して配布されますが、
// いかなる保証もありません。商品性や特定の目的に対する適合性の
// 暗黙的な保証さえもありません。詳細については、GNU劣等一般公衆
// 利用許諾契約書を参照してください。
//
// このライブラリと一緒にGNU劣等一般公衆利用許諾契約書の
// コピーを受け取っているはずです。受け取っていない場合は、
// フリーソフトウェア財団, 51 Franklin St, Fifth Floor,
// Boston, MA 02110-1301 USA までご連絡ください。

#ifndef __TM1637DISPLAY__
#define __TM1637DISPLAY__

#include <inttypes.h> // 整数型を使用するためのヘッダー

// セグメントのビット定義
#define SEG_A   0b00000001
#define SEG_B   0b00000010
#define SEG_C   0b00000100
#define SEG_D   0b00001000
#define SEG_E   0b00010000
#define SEG_F   0b00100000
#define SEG_G   0b01000000
#define SEG_DP  0b10000000 // ドットポイント（小数点）

// デフォルトのビット遅延（マイクロ秒）
#define DEFAULT_BIT_DELAY  100

// TM1637Displayクラスの定義
class TM1637Display {

public:
    // TM1637Displayオブジェクトを初期化し、クロックとデータピンを設定します。
    //
    // @param pinClk - モジュールのクロックピンに接続されたデジタルピンの番号
    // @param pinDIO - モジュールのDIOピンに接続されたデジタルピンの番号
    // @param bitDelay - ディスプレイに接続されたシリアルバスでのビット遷移間の遅延（マイクロ秒）
    TM1637Display(uint8_t pinClk, uint8_t pinDIO, unsigned int bitDelay = DEFAULT_BIT_DELAY);

    // ディスプレイの明るさを設定します。
    //
    // この設定は、表示されるデータを変更するコマンドが与えられたときに有効になります。
    //
    // @param brightness - 0（最も暗い）から7（最も明るい）までの数値
    // @param on - ディスプレイのオン/オフ（デフォルトはオン）
    void setBrightness(uint8_t brightness, bool on = true);

    // モジュールに任意のデータを表示します。
    //
    // この関数は、生のセグメント値を入力として受け取り、表示します。
    // セグメントデータはバイト配列で与えられ、各バイトが1つの桁に対応します。
    // 各バイト内で、ビット0はセグメントA、ビット1はセグメントBなどです。
    // この関数は、ディスプレイ全体、または任意の部分を設定できます。
    // 最初の桁は@ref pos引数で与えられ、0が最も左の桁です。
    // @ref length引数は、設定する桁数です。他の桁は影響を受けません。
    //
    // @param segments - 生のセグメント値を含む、サイズが@ref lengthの配列
    // @param length - 変更する桁数
    // @param pos - 変更を開始する位置（0 - 最も左、3 - 最も右）
    void setSegments(const uint8_t segments[], uint8_t length = 4, uint8_t pos = 0);

    // ディスプレイをクリアします。
    void clear();

    // 10進数を表示します。
    //
    // 与えられた引数を10進数として表示します。
    //
    // @param num - 表示する数値
    // @param leading_zero - trueの場合、先行ゼロ（先頭の不要な0）が表示されます。そうでない場合、不要な桁は空白になります。
    //                       注：負の数では先行ゼロはサポートされていません。
    // @param length - 設定する桁数。表示する数値が要求された桁数に収まるようにする必要があります
    //                 （例：2桁表示する場合、数値は0～99の範囲内である必要があります）。
    // @param pos - 最上位の桁の位置（0 - 最も左、3 - 最も右）
    void showNumberDec(int num, bool leading_zero = false, uint8_t length = 4, uint8_t pos = 0);

    // ドットを制御して10進数を表示します。
    //
    // 桁間のドット（またはコロン）を個別に制御して、与えられた引数を10進数として表示します。
    //
    // @param num - 表示する数値
    // @param dots - ドット/コロンの有効化。この引数はビットマスクであり、各ビットが桁間のドット
    //               （またはコロン）に対応します。例：
    //               各桁間にドットがあるディスプレイの場合：
    //               * 0.000 (0b10000000)
    //               * 00.00 (0b01000000)
    //               * 000.0 (0b00100000)
    //               * 0.0.0.0 (0b11100000)
    //               コロンのみのディスプレイの場合：
    //               * 00:00 (0b01000000)
    //               ドットとコロンがあるディスプレイの場合：
    //               * 0.0:0.0 (0b11100000)
    // @param leading_zero - trueの場合、先行ゼロが表示されます。そうでない場合、不要な桁は空白になります。
    //                       注：負の数では先行ゼロはサポートされていません。
    // @param length - 設定する桁数。表示する数値が要求された桁数に収まるようにする必要があります
    //                 （例：2桁表示する場合、数値は0～99の範囲内である必要があります）。
    // @param pos - 最上位の桁の位置（0 - 最も左、3 - 最も右）
    void showNumberDecEx(int num, uint8_t dots = 0, bool leading_zero = false, uint8_t length = 4, uint8_t pos = 0);

    // ドットを制御して16進数を表示します。
    //
    // 桁間のドット（またはコロン）を個別に制御して、与えられた引数を16進数として表示します。
    //
    // @param num - 表示する数値
    // @param dots - ドット/コロンの有効化。（詳細は`showNumberDecEx`と同様）
    // @param leading_zero - trueの場合、先行ゼロが表示されます。そうでない場合、不要な桁は空白になります。
    // @param length - 設定する桁数。表示する数値が要求された桁数に収まるようにする必要があります。
    // @param pos - 最上位の桁の位置（0 - 最も左、3 - 最も右）
    void showNumberHexEx(uint16_t num, uint8_t dots = 0, bool leading_zero = false, uint8_t length = 4, uint8_t pos = 0);

    // 1桁の数値を7セグメントコードに変換します。
    //
    // 0～15の数値を7セグメントディスプレイに表示するために必要なコードに変換します。
    // 10～15の数値は16進数の桁（A～F）に変換されます。
    //
    // @param digit - 0～15の数値
    // @return - 桁の7セグメント表示を表すコード（最下位ビット - セグメントA、
    //           ビット6 - セグメントG、ビット7 - 常にゼロ）
    static uint8_t encodeDigit(uint8_t digit);

protected:
    void bitDelay(); // ビット間の遅延

    void start(); // 通信開始

    void stop(); // 通信終了

    bool writeByte(uint8_t b); // 1バイトを書き込む

    void showDots(uint8_t dots, uint8_t* digits); // ドットを表示する
    
    void showNumberBaseEx(int8_t base, uint16_t num, uint8_t dots = 0, bool leading_zero = false, uint8_t length = 4, uint8_t pos = 0); // 任意の基数で数値を表示する

private:
    uint8_t m_pinClk; // クロックピン
    uint8_t m_pinDIO; // データピン
    uint8_t m_brightness; // 明るさ
    unsigned int m_bitDelay; // ビット遅延
};

#endif // __TM1637DISPLAY__
