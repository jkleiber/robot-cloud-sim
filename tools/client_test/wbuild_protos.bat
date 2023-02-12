@echo off

@REM mkdir protos
@REM xcopy /e /k /h /i /Y protos_init.py protos\__init__.py

:: Clean the protos directory
cd protos
git clean -dfX

:: Change back to this script's directory
cd %~dp0

:: Build the protobufs
pipenv run python -m grpc_tools.protoc -I../../src/boat --python_out=./protos --pyi_out=./protos --grpc_python_out=./protos ../../src/boat/*.proto
