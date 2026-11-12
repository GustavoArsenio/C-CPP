# Repositório de aprendizado de C e C++

Material educacional de C e C++ com lições progressivas, exercícios,
algoritmos reutilizáveis, pequenos projetos e metodologia de benchmarks. A
reestruturação e o currículo estão sendo desenvolvidos em etapas; as lacunas
atuais estão registradas na documentação.

## Primeiros passos

1. Instale compiladores de C e C++, CMake 3.20 ou mais recente e Make.
2. Configure e compile os exemplos portáveis:

   ```sh
   cmake -S . -B build
   cmake --build build
   ```

3. Execute as verificações com `ctest --test-dir build --output-on-failure`
   ou `make test`.

O cliente TCP POSIX e o exemplo de SHA-256 com OpenSSL são opcionais. Veja o
[guia de exemplos opcionais](docs/getting-started/optional-examples.md).

## Navegação

- [Trilha de C](docs/learning-paths/c.md)
- [Trilha de C++](docs/learning-paths/cpp.md)
- [Guia e resultado local de benchmark](benchmarks/README.md)
- [Catálogo de exercícios](exercises/catalog.json)
- [Auditoria do estado atual](docs/architecture/current-state.md)
- [Arquitetura proposta](docs/architecture/target-architecture.md)
- [Mapa da migração](docs/architecture/migration-map.md)
- [PDFs de enunciados preservados](docs/references/assignments/README.md)
- [Proposta e situação da licença](docs/architecture/licensing.md)
- [Roadmap](docs/roadmap.md)

O catálogo é a fonte única dos status e das contagens dos exercícios. Para
validá-lo e imprimir os totais, execute:

```sh
python3 tools/validation/catalog.py
```

O build padrão inclui testes unitários das pilhas encadeadas de inteiros e
caracteres, além de testes de saída para os exemplos catalogados.
Os testes do cliente TCP são ignorados em ambientes que bloqueiam sockets de
loopback.

A pasta histórica `C++/` continha programas em C, não uma trilha de C++.
Consulte o status de cada exercício antes de usar um exemplo como solução de
referência.
