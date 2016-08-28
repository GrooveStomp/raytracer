function has_arg() {
    local __args=($@)
    local argc=${#__args[@]}
    local last_arg=${__args[$argc-1]}
    local args=${__args[@]:0:$argc-1}

    local arg="$last_arg"
    if [[ -z "$arg" ]]; then return; fi

    for i in $args; do
        if [[ "$arg" = "$i" ]]; then
            echo "1"
            return
        fi
    done
}

function arg_index() {
    local __args=($@)
    local argc=${#__args[@]}
    local last_arg=${__args[$argc-1]}
    local args=${__args[@]:0:$argc-1}

    local arg="$last_arg"
    if [[ -z "$arg" ]]; then return; fi

    local index=0
    for i in $args; do
        if [[ "$arg" = "$i" ]]; then
            echo "$index"
            return
        else
            ((index++))
        fi
    done
}

function arg_at_index() {
    local __args=($@)
    local argc=${#__args[@]}
    local last_arg=${__args[$argc-1]}
    local args=${__args[@]:0:$argc-1}

    local wanted_index="$last_arg"
    if [[ -z "$wanted_index" ]]; then return; fi

    local index=0
    for i in $args; do
        if [[ "$index" = "$wanted_index" ]]; then
            echo "$i"
            return
        else
            ((index++))
        fi
    done
}

function get_arg_after() {
    local __args=($@)
    local argc=${#__args[@]}
    local last_arg=${__args[$argc-1]}
    local args=${__args[@]:0:$argc-1}

    local arg="$last_arg"
    if [[ -z "$arg" ]]; then return; fi

    local index=$(arg_index "$args" "$arg")
    if [[ -z "$index" ]]; then return; fi

    ((index++))
    echo $(arg_at_index "$args" "$index")
}

function test_arg_functions() {
    local __args=($@)
    local argc=${#__args[@]}

    local last_arg=${__args[$argc-1]}
    local args=${__args[@]:0:$argc-1}

    echo "has arg: $(has_arg $args $last_arg)"
    local index=$(arg_index $args $last_arg)
    echo "arg index: $index"
    echo "arg at index: $(arg_at_index $args $index)"
    echo "get arg after: $(get_arg_after $args $last_arg)"
}

function has_help_flag() {
    local args=$@

    if [[ $(has_arg $args "-h") || $(has_arg $args "--help") ]]; then
        echo "true"
        return
    fi
}
