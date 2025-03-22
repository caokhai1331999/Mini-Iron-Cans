git config --global --unset http.proxy
git fetch origin
git add src\*.cpp
git add include\*.h
git commit -m "latest update"
git push -f -u origin main
