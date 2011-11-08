find . -name '_page*' -type f -print | sed 's/\.\/_page\(.*\)/mv \.\/_page\1 \.\/page\1/' | sh

find . -type f -print0 | xargs -0 -n 1 sed -i -e 's/_page/page/g'
