#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suite in this file

#Check if 'ls' runs without errors
@test "Check 'ls' runs without errors" {
    run ./dsh <<EOF
ls
EOF
    [ "$status" -eq 0 ]
}

#Verify 'cd' changes the directory
@test "Verify 'cd' changes the directory" {
    run ./dsh <<EOF
cd /
pwd
EOF
    [ "$status" -eq 0 ]
    [ "$output" = "/" ]
}

#Ensure 'exit' command terminates the shell
@test "Ensure 'exit' command terminates the shell" {
    run ./dsh <<EOF
exit
EOF
    [ "$status" -eq 0 ]
}

#Execute a command with arguments
@test "Execute 'echo' with arguments" {
    run ./dsh <<EOF
echo Hello, World!
EOF
    [ "$status" -eq 0 ]
    [ "$output" = "Hello, World!" ]
}

#Handle a simple pipeline
@test "Handle a simple pipeline 'echo' and 'grep'" {
    run ./dsh <<EOF
echo Hello, World! | grep World
EOF
    [ "$status" -eq 0 ]
    [ "$output" = "Hello, World!" ]
}

#Handle a command with multiple arguments
@test "Handle 'ls' with '-l' and '-a' options" {
    run ./dsh <<EOF
ls -l -a
EOF
    [ "$status" -eq 0 ]
}

#Handle invalid command gracefully
@test "Handle invalid command gracefully" {
    run ./dsh <<EOF
invalidcommand
EOF
    [ "$status" -ne 0 ]
}

#Verify 'help' displays supported commands
@test "Verify 'help' displays supported commands" {
    run ./dsh <<EOF
help
EOF
    [ "$status" -eq 0 ]
    [[ "$output" == *"Supported commands: cd, exit, help"* ]]
}
