#!/bin/bash

find $1 -maxdepth 2 -name "init-repository.sh" -print0 | while read -d $'\0' file
do
    echo "$file"
    cp $1/Experiments/init-repository.sh $file
done

(cd $1 && git ws do -c "git commit -a -m 'update init-repository' ; git push")
