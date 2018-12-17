#!/bin/bash

for op in vmovsd vsubsd vaddsd vmulsd vfmadd[1-3]{3}sd vsqrtsd vdivsd ; do
  echo ${op}
  grep -E ${op} $1 | wc -l
done
