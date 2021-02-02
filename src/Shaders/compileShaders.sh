#yourfilenames=`ls ./*`
#for eachfile in $yourfilenames
#do
#  if [[ "$eachfile" == *.vert ]]
#  then
#    /bin/glslc "$eachfile" -o {${eachfile/.vert/.spv}}
#  fi
#  if [[ "$eachfile" == *.frag ]]
#  then
#    /bin/glslc "$eachfile" -o  {${eachfile/.frag/.spv}}
#  fi
#done
/bin/glslc VertexShader.vert -o vert.spv
/bin/glslc FragmentShader.frag -o frag.spv
/bin/glslc tessControlShader.tesc -o tessControl.spv
/bin/glslc tessEvaluationShader.tese -o tessEval.spv
/bin/glslc geometry.geom -o geometry.spv
