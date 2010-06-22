# diff from tlp files
# scene descriptions are removed before diff

# remove old files 
if [ -f tlp_diff_arg1.tlp ]; then
    rm tlp_diff_arg1.tlp
fi

if [ -f tlp_diff_arg2.tlp ]; then
    rm tlp_diff_arg2.tlp
fi

# remove date, comments
sed '/(date /,/")/d' $1 > tlp_diff_arg1.tlp
sed '/(date /,/")/d' $2 > tlp_diff_arg2.tlp

# and scene descriptions if needed
if [ "$TLP_DIFF_SCENE" = "NO" ]; then
  sed '/<scene/,/<\/scene>/d' tlp_diff_arg1.tlp > tlp_diff2_arg1.tlp
  mv tlp_diff2_arg1.tlp tlp_diff_arg1.tlp
  sed '/<scene/,/<\/scene>/d' tlp_diff_arg2.tlp > tlp_diff2_arg2.tlp
  mv tlp_diff2_arg2.tlp tlp_diff_arg2.tlp
fi

diff tlp_diff_arg1.tlp tlp_diff_arg2.tlp

# remove created files
rm tlp_diff_arg1.tlp tlp_diff_arg2.tlp
