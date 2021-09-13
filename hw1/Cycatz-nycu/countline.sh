#!/usr/bin/env bash

main() {
    if [[ $# -lt 1 ]]; then
        printf 'missing file name\n'
    elif [[ $# -gt 1 ]]; then
        printf 'only one argument is allowed\n'
    else
        local fname="$1"
        if [[ -e "${fname}" ]]; then
            local line_cnt=$(wc -l "${fname}" | awk '{print $1}')
            printf '%s lines in %s\n' "${line_cnt}" "${fname}"
        else
            printf '%s not found\n' "${fname}"
        fi
    fi
}

main "$@"
