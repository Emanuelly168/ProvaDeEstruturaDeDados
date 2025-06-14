#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MaximoNumeroDeSensores 100
#define MaximoNumeroDeLeituras 10000
#define MaximoTamanhoID 32
#define MaximoTamanhoValor 32

typedef struct {
    long timestamp;
    char valor[MaximoTamanhoValor];
} Entrada;

typedef struct {
    char identificador[MaximoTamanhoID];
    Entrada dados[MaximoNumeroDeLeituras];
    int quantidade;
} DadosDoSensor;

int CompararDados(const void* a, const void* b) {
    return ((Entrada*)a)->timestamp - ((Entrada*)b)->timestamp;
}

int BuscarSensor(DadosDoSensor sensores[], int count, const char* identificador) {
    for (int i = 0; i < count; i++) {
        if (strcmp(sensores[i].identificador, identificador) == 0) return i;
    }
    return -1;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Uso: %s <nome do arquivo>\n", argv[0]); 
        return 1;
    }

    FILE* f = fopen(argv[1], "r");
    if (!f) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    DadosDoSensor sensores[MaximoNumeroDeSensores];
    int NumeroTotalDeSensores = 0;

    char linha[128], identificador[MaximoTamanhoID], valor[MaximoTamanhoValor];
    long timestamp;

    while (fgets(linha, sizeof(linha), f)) {
        if (sscanf(linha, "%ld %s %s", &timestamp, identificador, valor) != 3) continue;

        int idx = BuscarSensor(sensores, NumeroTotalDeSensores, identificador);
        if (idx == -1) {
            if (NumeroTotalDeSensores >= MaximoNumeroDeSensores) {
                printf("Erro: Número máximo de sensores atingido!\n");
                fclose(f);
                return 1;
            }
            idx = NumeroTotalDeSensores++;
            strcpy(sensores[idx].identificador, identificador);
            sensores[idx].quantidade = 0;
        }

        DadosDoSensor* s = &sensores[idx];
        if (s->quantidade < MaximoNumeroDeLeituras) {
            s->dados[s->quantidade].timestamp = timestamp;
            strcpy(s->dados[s->quantidade].valor, valor);
            s->quantidade++;
        } else {
            printf("Aviso: Número máximo de leituras atingido para o sensor %s!\n", identificador);
        }
    }
    fclose(f);

    for (int i = 0; i < NumeroTotalDeSensores; i++) {
        DadosDoSensor* s = &sensores[i];
        qsort(s->dados, s->quantidade, sizeof(Entrada), CompararDados);

        char ArquivoDeSaida[64];
        snprintf(ArquivoDeSaida, sizeof(ArquivoDeSaida), "%s.txt", s->identificador);
        FILE* out = fopen(ArquivoDeSaida, "w");
        if (!out) {
            printf("Erro ao criar arquivo: %s\n", ArquivoDeSaida);
            continue;
        }

        for (int j = 0; j < s->quantidade; j++) {
            fprintf(out, "%ld %s\n", s->dados[j].timestamp, s->dados[j].valor);
        }
        fclose(out);
        printf("Arquivo criado com sucesso: %s (%d dados)\n", ArquivoDeSaida, s->quantidade);
    }

    return 0;
}
