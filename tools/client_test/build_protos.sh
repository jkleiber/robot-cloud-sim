
mkdir protos
cp protos_init.py protos/__init__.py
pipenv run python -m grpc_tools.protoc -I../../src/boat --python_out=./protos --pyi_out=./protos --grpc_python_out=./protos ../../src/boat/*.proto
