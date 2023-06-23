const database = require('../models')
// const Sequelize = require('sequelize')
const { TagsServices } = require('../services')
const tagServices = new TagsServices()

class TagController {
  // Busca todos os Registros ativos

  static async pegaTodasAsPessoasAtivas(req, res) {
    try {
      const pessoasAtivas = await tagServices.pegaRegistrosAtivos()
      return res.status(200).json(pessoasAtivas)
    } catch (error) {
      return res.status(500).json(error.message)
    }
  }

  // Busca todos os Registros
  static async pegaTodasAsTags(req, res) {
    try {
      const todasAsPessoas = await tagServices.pegaTodosOsRegistros()
      const result = todasAsPessoas[0]
      return res.status(200).json(result)
    } catch (error) {
      return res.status(500).json(error.message)
    }
  }

  //Busca Tag por Id
  static async pegaUmaTag(req, res) {
    const { id } = req.params
    try {
      const buscaPorId = await tagServices.pegaPorId(id)
      // const umaPessoa = await database.Pessoas.findOne({
      //   where: {
      //     id: Number(id)
      //   }
      // })
      return res.status(200).json(buscaPorId)
    } catch (error) {
      return res.status(500).json(error.message)
    }
  }

  // Criação de Registro
  static async criaTag(req, res) {
    const novaTag = req.body
    try {
      const novaTagCriada = await tagServices.criaRegistro(novaTag)
      return res.status(200).json(novaTagCriada)
      // if (typeof req.body.email == 'string') {
      //   const novaPessoaCriada = await database.Pessoas.create(novaPessoa)
      //   return res.status(200).json(novaPessoaCriada)
      // }
      // return res.status(400).json({
      //   mensagem: `O valor passado no campo email está incorreto, favor informar novamente!`
      // })
    } catch (error) {
      return res.status(500).json(error.message)
    }
  }

  //Atualizar um registro
  static async atualizarTag(req, res) {
    const { id } = req.params
    const novasInfos = req.body
    try {
      await database.Tags.update(novasInfos, { where: { id: Number(id) } })
      const tagAtualizada = await database.Tags.findOne({
        where: {
          id: Number(id)
        }
      })
      return res.status(200).json(tagAtualizada)
    } catch (error) {
      return res.status(500).json(error.message)
    }
  }

  //Deletar um Registro
  static async apagaTag(req, res) {
    const { id } = req.params

    try {
      await database.Tags.destroy({ where: { id: (id) } })
      return res.status(200).json({ mensagem: `id ${id} deletado!` })
    } catch (error) {
      return res.status(500).json(error.message)
    }
  }

  //Restaura registro
  static async restauraPessoa(req, res) {
    const { id } = req.params

    try {
      await database.Tags.restore({ where: { id: Number(id) } })
      return res.status(200).json({ mensagem: `id ${id} restaurado!` })
    } catch (error) {
      return res.status(500).json(error.message)
    }
  }

  // Pega uma Matricula (Modelo convencional, o qual eu fiz antes de assistir a aula)
  static async pegaUmaMatricula(req, res) {
    const { estudanteId, matriculaId } = req.params
    try {
      const umaMatricula = await database.Matriculas.findOne({
        where: {
          id: Number(matriculaId),
          estudante_Id: Number(estudanteId)
        }
      })
      return res.status(200).json(umaMatricula)
    } catch (error) {
      return res.status(500).json(error.message)
    }
  }

  // Pega uma Matricula
  static async pegaMatriculaConfirmada(req, res) {
    const { estudanteId } = req.params
    try {
      const pessoa = await database.Pessoas.findOne({
        where: { id: Number(estudanteId) }
      })
      const matriculas = await pessoa.getAulasMatriculadas()
      return res.status(200).json(matriculas)
    } catch (error) {
      return res.status(500).json(error.message)
    }
  }

  static async pegaMatriculasPorTurma(req, res) {
    const { turmaId } = req.params
    try {
      const todasAsMatriculas = await database.Matriculas.findAndCountAll({
        where: {
          turma_id: Number(turmaId),
          status: 'confirmado'
        },
        limit: 20,
        order: [['estudante_id', 'DESC']]
      })
      return res.status(200).json(todasAsMatriculas)
    } catch (error) {
      return res.status(500).json(error.message)
    }
  }

  static async pegaTurmasLotadas(req, res) {
    const lotacaoTurma = 2
    try {
      const turmasLotadas = await database.Matriculas.findAndCountAll({
        where: {
          status: 'confirmado'
        },
        attributes: ['turma_id'],
        group: ['turma_id'],
        having: Sequelize.literal(`count(turma_id) >= ${lotacaoTurma}`)
      })
      return res.status(200).json(turmasLotadas.count)
    } catch (error) {
      return res.status(500).json(error.message)
    }
  }

  //Cancela Matricula de Usuario
  static async cancelaPessoa(req, res) {
    const { estudanteId } = req.params
    try {
      await tagServices.cancelaPessoasEMatriculas(Number(estudanteId))
      return res
        .status(200)
        .json({ message: `Matriculas referente ao ${estudanteId} canceladas` })
    } catch (error) {
      return res.status(500).json(error.message)
    }
  }

  // Restaura Matricula de Usuario
  static async restauraMatriculaPessoa(req, res) {
    const { estudanteId } = req.params
    try {
      database.sequelize.transaction(async transacao => {
        await database.Pessoas.scope('todos').update(
          { ativo: true },
          {
            where: { id: Number(estudanteId) }
          },
          { transaction: transacao }
        )
        await database.Matriculas.update(
          { status: 'confirmado' },
          {
            where: { estudante_id: Number(estudanteId) }
          },
          { transaction: transacao }
        )
      })
      return res
        .status(200)
        .json({ message: `Matriculas referente ao ${estudanteId} restauradas` })
    } catch (error) {
      return res.status(500).json(error.message)
    }
  }

  // // Pega uma Matricula (Modelo convencional, o qual eu fiz antes de assistir a aula)
  // static async pegaMatriculaConfirmada(req, res) {
  //   const { estudanteId } = req.params
  //   try {
  //     const matriculasConfirmadas = await database.Matriculas.findAll({
  //       where: {
  //         estudante_Id: Number(estudanteId),
  //         status: String('confirmado')
  //       }
  //     })
  //     return res.status(200).json(matriculasConfirmadas)
  //   } catch (error) {
  //     return res.status(500).json(error.message)
  //   }
  // }

  // Cria Matricula
  static async criaMatricula(req, res) {
    const { estudanteId } = req.params
    const novaMatricula = { ...req.body, estudante_id: Number(estudanteId) }
    try {
      const novaMatriculaCriada = await database.Matriculas.create(
        novaMatricula
      )
      return res.status(200).json(novaMatriculaCriada)
    } catch (error) {
      return res.status(500).json(error.message)
    }
  }

  //Atualiza Matricula
  static async atualizarMatricula(req, res) {
    const { estudanteId, matriculaId } = req.params
    const novasInfos = req.body
    try {
      await database.Matriculas.update(novasInfos, {
        where: { id: Number(matriculaId), estudante_id: Number(estudanteId) }
      })
      const matriculaAtualizada = await database.Matriculas.findOne({
        where: {
          id: Number(matriculaId)
        }
      })
      return res.status(200).json(matriculaAtualizada)
    } catch (error) {
      return res.status(500).json(error.message)
    }
  }

  //Deletar uma Matricula
  static async apagaMatricula(req, res) {
    const { estudanteId, matriculaId } = req.params

    try {
      await database.Matriculas.destroy({
        where: { id: Number(matriculaId), estudante_id: Number(estudanteId) }
      })
      return res.status(200).json({ mensagem: `id ${matriculaId} deletado!` })
    } catch (error) {
      return res.status(500).json(error.message)
    }
  }
}

module.exports = TagController
