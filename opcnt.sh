#!/bin/bash

for op in vsubsd vaddsd vmulsd vfmadd[1-3]{3}sd vsqrtsd vdivsd vmovsd; do
  echo ${op}
  grep -E ${op} $1 | wc -l
done
