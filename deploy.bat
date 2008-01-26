@echo off

echo Uploading nlink to maven repositry.
echo [[Attention]] Please execute on nlink project's directory.
echo Can't execute who don't have a seasar.org's maven account.

pause

set MVN=call mvn deploy:deploy-file -Dpackaging=jar -DrepositoryId=maven.seasar.org -Durl=dav:https://www.seasar.org/maven/maven2
set GROUPID=-DgroupId=nlink
set VER=-Dversion=20080126

%MVN% %GROUPID% %VER% -DartifactId=nlink -Dfile=.\runtime\build\nlink.jar

pause
