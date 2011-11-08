find . -name '_page*' -type f -print | sed 's/\.\/_page\(.*\)/mv \.\/_page\1 \.\/page\1/' | sh
find . -type f -print0 | xargs -0 -n 1 sed -i -e 's/_page/page/g'

find . -name '____init*' -type f -print | sed 's/\.\/____init\(.*\)/mv \.\/____init\1 \.\/init\1/' | sh
find . -type f -print0 | xargs -0 -n 1 sed -i -e 's/____init/init/g'
