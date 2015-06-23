#!/bin/bash

tf=$(mktemp)
$1 $2 > $tf
diff $tf $3 > /dev/null

