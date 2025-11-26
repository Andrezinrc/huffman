# huffman

# ⚠️ AVISO LEGAL

Este projeto é **exclusivamente educacional**.  
Embora funcional, **não é um compressor profissional** e **não deve ser usado para fins críticos ou em produção**.

O autor **não se responsabiliza** por:  
- Perda de arquivos ou corrupção de dados  
- Uso inadequado do software  
- Danos diretos ou indiretos decorrentes do uso  

Use **por sua própria conta e risco**.

---

Projeto em C para estudar compressão e descompressão usando o **algoritmo de Huffman**.  
Implementa contagem de frequência, árvore de Huffman, geração de códigos, compressão e descompressão de arquivos.

## Compilar
```bash
gcc main.c huffman.c -o huffman
```

## Executar

```bash
./huffman compress <arquivo_entrada> <arquivo_saida.adr>
./huffman decompress <arquivo.adr> <arquivo_saida>
```

O programa permite compactar arquivos para .adr e descompactar mantendo os bytes originais e a extensão do arquivo.