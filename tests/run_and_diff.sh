#!/bin/bash

tf=$(mktemp)
$1 > $tf
diff $tf $2 > /dev/null

