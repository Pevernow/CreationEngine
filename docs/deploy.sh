
rm -rf .vuepress/dist
mkdir .vuepress/dist
yarn vuepress build

cd .vuepress/dist

git init
git add -A
git commit -m 'deploy'

git push -f git@github.com:Pevernow/CreationEngine.git master:gh-pages

cd ../..

