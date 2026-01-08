🔧 C/C++ → 全般 → 追加のインクルード ディレクトリ
(x64)
$(ProjectDir)third_party\openssl\x64\include;$(ProjectDir)third_party\libsodium\include
(ARM64)
$(ProjectDir)third_party\openssl\arm64\include;$(ProjectDir)third_party\libsodium\include

🔧 リンカー → 全般 → 追加のライブラリ ディレクトリ
(x64)
$(ProjectDir)third_party\openssl\x64\lib;$(ProjectDir)third_party\libsodium\x64\Debug\v143\dynamic
(ARM64)
$(ProjectDir)third_party\openssl\arm64\lib;$(ProjectDir)third_party\libsodium\ARM64\Debug\v143\dynamic

🔧 リンカー → 入力 → 追加の依存ファイル
libcrypto.lib;libssl.lib;libsodium.lib;$(CoreLibraryDependencies);%(AdditionalDependencies)

▶ 実行時
- secutiry\secutiry\third_party\openssl\x64\bin\libcrypto-3-x64.dll
- secutiry\secutiry\third_party\openssl\x64\bin\libssl-3-x64.dll
- secutiry\secutiry\third_party\libsodium\x64\Debug\v143\dynamic\libsodium.dll

以上を実行後できたプロジェクトルートのx64\Debugフォルダにコピーする
