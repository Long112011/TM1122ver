del *.ncb *.sbr *.obj *.pdb *.idb *.ib_pdb_index *.pch *.ib_tag *.embed.manifest *.user *.exp *.ilk *.map *.intermediate.manifest *.plg *.dep *.htm *.xml *.cod *.dmp *.tlog *.lastbuildstate *.log *.cache *.rsp *.res *.bsc *.ipch *.unsuccessfulbuild /s >> delete_output.txt
FOR /F delims^= %%A IN ('DIR/AD/B/S^|SORT/R') DO RD "%%A" >> delete_output.txt
