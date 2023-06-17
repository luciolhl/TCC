const database = require('../models')

const Services = require('../services/Services')
const categoriasServices = new Services('Categorias')

class CategoriaController {
  //Busca de todos os Categorias
  static async pegaTodosOscategorias(req, res) {
    try {
      const todosOsCategorias = await categoriasServices.pegaTodosOsRegistros()
      return res.status(200).json(todosOsCategorias)
    } catch (error) {
      return res.status(500).json(error.message)
    }
  }

  //Busca de nivel por id
  static async pegaUmaCategoria(req, res) {
    const { id } = req.params
    try {
      const umaCategoria = await categoriasServices.pegaPorId(id)
      return res.status(200).json(umaCategoria)
    } catch (error) {
      return res.status(500).json(error.message)
    }
  }

  //Criação de Registro Nivel
  static async criaCategoria(req, res) {
    const novaCategoria = req.body
    try {
      const novaCategoriaCriada = await categoriasServices.criaRegistro(novaCategoria)
      return res.status(200).json(novaCategoriaCriada)
    } catch (error) {
      return res.status(500).json(error.message)
    }
  }

  //Atualizar um registro
  static async atualizarCategoria(req, res) {
    const { id } = req.params
    const novasInfos = req.body
    try {
      await database.Categorias.update(novasInfos, { where: { id: Number(id) } })
      const nivelAtualizado = await database.Categorias.findOne({
        where: {
          id: Number(id)
        }
      })
      return res.status(200).json(nivelAtualizado)
    } catch (error) {
      return res.status(500).json(error.message)
    }
  }

  //Deletar um Registro
  static async apagaCategoria(req, res) {
    const { id } = req.params

    try {
      await database.Categorias.destroy({ where: { id: Number(id) } })
      return res.status(200).json({ mensagem: `id ${id} deletado!` })
    } catch (error) {
      return res.status(500).json(error.message)
    }
  }

  static async restauraCategoria(req, res) {
    const { id } = req.params

    try {
      await database.Categorias.restore({ where: { id: Number(id) } })
      return res.status(200).json({ mensagem: `id ${id} restaurado!` })
    } catch (error) {
      return res.status(500).json(error.message)
    }
  }
}

module.exports = CategoriaController
