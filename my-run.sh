git pull
echo "参数个数为：$#"
./gradlew clean
adb shell am force-stop com.sohu.ping
adb uninstall com.sohu.ping
./gradlew assembledebug
adb install app/build/outputs/apk/debug/app-debug.apk
adb shell am start -n com.sohu.ping/com.sohu.ping.MainActivity
