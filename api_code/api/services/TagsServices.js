const Services = require('./Services')
const database = require('../models')

class TagsService extends Services {
  constructor() {
    super('Tags')
  }
  // métodos especificos do controlador de Tags

  // async pegaRegistrosAtivos(where = {}) {
  //   return database[this.nomeDoModelo].sequelize.query("Select * from Tags t inner join Categorias c on t.categoria_id = c.id;");
  // }

  async pegaTodosOsRegistros(where = {}) {
    return database[this.nomeDoModelo].sequelize.query("Select t.id as ID, t.local as Local, c.categorias as Categoria, c.item as Item from Tags t inner join Categorias c on t.categoria_id = c.id;");
  }

  async cancelaTagsEMatriculas(estudanteId) {
    return database.sequelize.transaction(async transacao => {
      await super.atualizaRegistro({ ativo: false }, estudanteId, {
        transaction: transacao
      })
      await this.matriculas.atualizaRegistros(
        { status: 'cancelado' },
        { estudante_id: estudanteId },
        { transaction: transacao }
      )
    })
  }

  async pegaMatriculasPorEstudante(where = {}) {
    const matriculas = await database[this.nomeDoModelo].findOne({
      where: { ...where }
    })
    return matriculas.getAulasMatriculadas()
  }
}

module.exports = TagsService
