#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suit in this file

@test "Example: check ls runs without errors" {
    run ./dsh <<EOF                
ls
EOF

    # Assertions
    [ "$status" -eq 0 ]
}

# Test case: Check if 'ls' handles quoted spaces correctly
@test "Check if 'ls' handles quoted spaces correctly" {
    run ./dsh <<EOF
ls "path with spaces"
EOF
    [ "$status" -eq 0 ]
}

# Test case: Check if 'cd' changes directories correctly
@test "Check if 'cd' changes directories correctly" {
    run ./dsh <<EOF
cd /tmp
EOF
    [ "$status" -eq 0 ]
}

# Test case: Check if 'exit' command terminates the shell
@test "Check if 'exit' command terminates the shell" {
    run ./dsh <<EOF
exit
EOF
    [ "$status" -eq 0 ]
}

# Test case: Check if 'cd' handles missing argument
@test "Check if 'cd' handles missing argument" {
    run ./dsh <<EOF
cd
EOF
    [ "$status" -eq 1 ]
}

# Test case: Check if 'cd' handles invalid directory
@test "Check if 'cd' handles invalid directory" {
    run ./dsh <<EOF
cd /nonexistent_directory
EOF
    [ "$status" -eq 1 ]
}

# Test case: Check if 'ls' handles invalid options
@test "Check if 'ls' handles invalid options" {
    run ./dsh <<EOF
ls --invalid-option
EOF
    [ "$status" -eq 1 ]
}