@echo off

@REM mkdir protos
@REM xcopy /e /k /h /i /Y protos_init.py protos\__init__.py
pipenv run python -m grpc_tools.protoc -I../../src/msg --python_out=./protos --pyi_out=./protos --grpc_python_out=./protos ../../src/msg/*.proto
