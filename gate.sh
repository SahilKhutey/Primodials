#!/bin/bash
# gate.sh - Pre-release validation gate

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

echo "╔═══════════════════════════════════════════════════════════════╗"
echo "║   PRE-RELEASE VALIDATION GATE                                ║"
echo "╚═══════════════════════════════════════════════════════════════╝"
echo ""

FAILED=0

check() {
    local description="$1"
    local command="$2"
    
    echo -n "  $description... "
    if eval "$command" > /dev/null 2>&1; then
        echo -e "${GREEN}✓${NC}"
        return 0
    else
        echo -e "${RED}✗${NC}"
        ((FAILED++))
        return 1
    fi
}

echo "═══ Package Structure ═══"
check "All required files exist" "test -f README.md && test -f LICENSE && test -f CHANGELOG.md && test -f build_all.sh"
check "Developer tools present" "test -f dev/setup_dev.sh && test -d dev/scripts && test -d dev/vscode"
check "Installer sources present" "test -f installer/windows/installer.iss && test -f installer/linux/build_deb.sh"

echo ""
echo "═══ Content Validation ═══"
check "Themes exist" "test -d Content/Themes || test -d source/Content/Themes"
check "Locales available" "test -d Content/Locales || test -d source/Content/Locales"
check "Accessibility checklist present" "test -f accessibility_checklist.md"
check "Ship readiness report present" "test -f ship_readiness_report.md"

if [ $FAILED -eq 0 ]; then
    echo ""
    echo -e "  ${GREEN}✓ READY TO SHIP${NC}"
    exit 0
else
    echo ""
    echo -e "  ${RED}✗ NOT READY - FIX ERRORS${NC}"
    exit 1
fi
