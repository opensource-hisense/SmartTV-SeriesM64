REVISION = '$Change: 925091 $'

try:
    import re
    REVISION = re.search(r'\d+', REVISION).group(0)
except AttributeError:
    REVISION = '0'
