# diff from tlp files
# scene descriptions are removed before diff

# remove old files 
if [ -f tlp_diff_arg1.tlp ]; then
    rm tlp_diff_arg1.tlp
fi

if [ -f tlp_diff_arg2.tlp ]; then
    rm tlp_diff_arg2.tlp
fi

# remove tlp, date, comments and viewFontAwesomeIcon property
sed -e '/(tlp /,/Tulip.")/d' -e '/(property  0 string "viewFontAwesomeIcon"/ {
N
N
d}' -e '/(property  0 string "viewIcon"/ {
N
N
d}' -e '/(graph_attributes /,/^)/d' $1 > tlp_diff_arg1.tlp
sed -e '/(tlp /,/Tulip.")/d' -e '/(property  0 string "viewFontAwesomeIcon"/ {
N
N
d}' -e '/(property  0 string "viewIcon"/ {
N
N
d}' -e '/(graph_attributes /,/^)/d' $2 > tlp_diff_arg2.tlp

# and controller description if needed
if [ "$TLP_DIFF_CONTROLLER" = "NO" ]; then
  sed '/(controller /,$d' tlp_diff_arg1.tlp > tlp_diff2_arg1.tlp
  mv tlp_diff2_arg1.tlp tlp_diff_arg1.tlp
  sed '/(controller /,$d' tlp_diff_arg2.tlp > tlp_diff2_arg2.tlp
  mv tlp_diff2_arg2.tlp tlp_diff_arg2.tlp
fi

diff tlp_diff_arg1.tlp tlp_diff_arg2.tlp

ok=$?

# remove created files
rm tlp_diff_arg*.tlp

exit $ok
